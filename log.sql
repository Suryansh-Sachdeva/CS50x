-- Keep a log of any SQL queries you execute as you solve the mystery.
SELECT description FROM crime_scene_reports
WHERE year = 2020 AND month = 7 AND day =  28 AND street = 'Chamberlin Street';

SELECT name, transcript FROM interviews
WHERE year = 2020 AND month = 7 AND day =  28 AND transcript LIKE '%courthouse%';

SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day =  28 AND hour = 10
AND activity = 'exit' AND minute BETWEEN 15 AND 25;

-- People who have these cards:
SELECT name FROM people
WHERE license_plate IN (
SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day =  28 AND hour = 10
AND activity = 'exit' AND minute BETWEEN 15 AND 25);

-- Based on Raymond's testimony, I looked at the calls made after the theft and with less
-- one minute, looking for the name among the suspects who have signs that were in placel
SELECT * FROM people
WHERE license_plate IN (
SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day =  28 AND hour = 10
AND activity = 'exit' AND minute BETWEEN 15 AND 25)
AND phone_number IN (
SELECT caller FROM phone_calls
WHERE year = 2020 AND month = 7 AND day =  28 AND duration <= 60);

SELECT caller, receiver FROM phone_calls
WHERE year = 2020 AND month = 7 AND day =  28 AND duration <= 60
AND caller IN(
SELECT phone_number FROM people
WHERE license_plate IN (
SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day =  28 AND hour = 10
AND activity = 'exit' AND minute BETWEEN 15 AND 25)
AND phone_number IN (
SELECT caller FROM phone_calls
WHERE year = 2020 AND month = 7 AND day =  28 AND duration <= 60)
);


SELECT * FROM people
WHERE phone_number = '(996) 555-8899' OR phone_number = '(892) 555-8872'
OR phone_number = '(375) 555-8161' OR phone_number = '(717) 555-1342'
OR phone_number = '(725) 555-3243';


SELECT flights.id FROM flights
JOIN airports ON airports.id = flights.origin_airport_id
WHERE year = 2020 AND month = 7 AND day =  29 AND city = 'Fiftyville'
ORDER BY hour, minute LIMIT 1;

SELECT name FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN flights ON flights.id = passengers.flight_id
WHERE flights.id = 36 AND passengers.passport_number IN (
SELECT passport_number FROM people
WHERE name = 'Roger' OR name = 'Russell'
OR name = 'Evelyn' OR name = 'Ernest');

SELECT name FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN flights ON flights.id = passengers.flight_id
WHERE flights.id = 36 AND passengers.passport_number IN (
SELECT passport_number FROM people
WHERE name = 'Larry' OR name = 'Jack' OR name = 'Melissa'
OR name = 'Philip' OR name = 'Berthold');


SELECT city FROM airports
WHERE id = (
SELECT destination_airport_id FROM flights
JOIN airports ON airports.id = flights.origin_airport_id
WHERE flights.id = 36);


