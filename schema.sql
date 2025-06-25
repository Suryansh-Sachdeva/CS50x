-- schema.sql
-- Drop tables if they exist (to avoid "table already exists" errors)
DROP TABLE IF EXISTS history;
DROP TABLE IF EXISTS stocks;
DROP TABLE IF EXISTS users;

-- Users table
CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    hash TEXT NOT NULL,
    cash REAL NOT NULL DEFAULT 0.0
);

-- Stocks table
CREATE TABLE stocks (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    shares INTEGER NOT NULL CHECK(shares >= 0),
    FOREIGN KEY (user_id) REFERENCES users(id)
);

-- History table
CREATE TABLE history (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    shares INTEGER NOT NULL,
    price REAL NOT NULL,
    action TEXT NOT NULL CHECK(action IN ('buy', 'sell')),
    timestamp TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id)
);
