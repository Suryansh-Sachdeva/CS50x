import os
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Connect to database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    stocks = db.execute("SELECT symbol, shares FROM stocks WHERE user_id = ? GROUP BY symbol", session["user_id"])
    total_money = 0

    for stock in stocks:
        api = lookup(stock["symbol"])
        if not api:
            return apology("Invalid stock data", 400)
        stock["name"] = api["name"]
        stock["price"] = api["price"]
        stock["total"] = stock["price"] * stock["shares"]
        total_money += stock["total"]

    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
    total_money += cash

    return render_template("index.html", stocks=stocks, cash=cash, total=total_money)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("must provide stock symbol", 400)

        stock = lookup(symbol.upper())
        if stock is None:
            return apology("invalid symbol", 400)

        if not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("invalid number of shares", 400)

        shares = int(shares)
        price = stock["price"]
        total = shares * price

        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
        if total > cash:
            return apology("not enough cash", 400)

        db.execute("INSERT INTO history (user_id, symbol, shares, price, action, timestamp) VALUES (?, ?, ?, ?, 'buy', datetime('now'))",
                   session["user_id"], stock["symbol"], shares, price)

        row = db.execute("SELECT shares FROM stocks WHERE user_id = ? AND symbol = ?", session["user_id"], stock["symbol"])
        if row:
            db.execute("UPDATE stocks SET shares = ? WHERE user_id = ? AND symbol = ?",
                       row[0]["shares"] + shares, session["user_id"], stock["symbol"])
        else:
            db.execute("INSERT INTO stocks (user_id, symbol, shares) VALUES (?, ?, ?)",
                       session["user_id"], stock["symbol"], shares)

        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash - total, session["user_id"])
        flash("Bought!")
        return redirect("/")

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    transactions = db.execute("SELECT * FROM history WHERE user_id = ?", session["user_id"])
    for tx in transactions:
        sign = "-" if tx["action"] == "buy" else "+"
        tx["total"] = sign + usd(tx["price"] * tx["shares"])
    return render_template("history.html", stocks=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    session.clear()

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")

        if not username:
            return apology("must provide username", 400)
        if not password:
            return apology("must provide password", 400)

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("invalid username and/or password", 400)

        session["user_id"] = rows[0]["id"]
        flash("Logged in!")
        return redirect("/")

    return render_template("login.html")


@app.route("/logout")
def logout():
    session.clear()
    flash("Logged out!")
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide symbol", 400)

        stock = lookup(symbol.upper())
        if not stock:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", stock=stock)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username or not password or not confirmation:
            return apology("must fill all fields", 400)
        if password != confirmation:
            return apology("passwords don't match", 400)
        if db.execute("SELECT * FROM users WHERE username = ?", username):
            return apology("username already exists", 400)

        hash_pw = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash_pw)
        session["user_id"] = db.execute("SELECT id FROM users WHERE username = ?", username)[0]["id"]

        flash("Registered!")
        return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    rows = db.execute("SELECT symbol, shares FROM stocks WHERE user_id = ?", session["user_id"])

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol or not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("invalid input", 400)

        shares = int(shares)

        owned = next((row for row in rows if row["symbol"] == symbol), None)
        if not owned or shares > owned["shares"]:
            return apology("too many shares", 400)

        stock = lookup(symbol)
        if not stock:
            return apology("invalid stock", 400)

        total = shares * stock["price"]

        if shares == owned["shares"]:
            db.execute("DELETE FROM stocks WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
        else:
            db.execute("UPDATE stocks SET shares = ? WHERE user_id = ? AND symbol = ?",
                       owned["shares"] - shares, session["user_id"], symbol)

        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + total, session["user_id"])

        db.execute("INSERT INTO history (user_id, symbol, shares, price, action, timestamp) VALUES (?, ?, ?, ?, 'sell', datetime('now'))",
                   session["user_id"], symbol, shares, stock["price"])

        flash("Sold!")
        return redirect("/")

    return render_template("sell.html", stocks=rows)


@app.route("/changecash", methods=["POST"])
@login_required
def change_cash():
    try:
        amount = float(request.form.get("amount"))
    except (ValueError, TypeError):
        return apology("invalid amount", 400)

    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
    db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + amount, session["user_id"])
    flash("Cash updated!")
    return redirect("/")


@app.route("/greet", methods=["POST"])
@login_required
def greet():
    try:
        cash = float(request.form.get("cash"))
    except (ValueError, TypeError):
        return apology("invalid amount", 400)

    db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"])
    return redirect("/")


def errorhandler(e):
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)

for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
