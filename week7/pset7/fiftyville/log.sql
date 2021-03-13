-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Theft took place on July 28, 2020 and that it took place on Chamberlin Street.

-- Start by checking crime_scene_reports on July 28, 2020
SELECT description
FROM crime_scene_reports
WHERE day = 28
AND month = 7
AND year = 2020
AND street = "Chamberlin Street";
-- 10:15am at the Chamberlin Street courthouse.
-- Interviews same-day with three witnesses mention courthouse

-- Find name and transcripts of each witness
SELECT name, transcript
FROM interviews
WHERE day = 28
AND month = 7
AND year = 2020;
-- Ruth | Car in the courthouse parking lot and drive away within 10:15-10:25am
-- Eugene | Eugene recognized. Saw at ATM in morning on Fifer Street withdrawing money.
-- Raymond | Called someone minute after to purchase earliest flight out of Fiftyville (29th).

-- Check the security logs at 10:15am<>10:25am at Chamberlin Street courthouse
SELECT license_plate
FROM courthouse_security_logs
WHERE day = 28
AND month = 7
AND year = 2020
AND hour = 10
AND minute >= 15
AND minute <= 25
AND activity = "exit";
-- Results:
-- 5P2BI95
-- 94KL13X
-- 6P58WS2
-- 4328GD8
-- G412CB7
-- L93JTIZ
-- 322W7JE
-- 0NTHK55

-- Check ATM on Fifer Street in the morning for withdrawrals
SELECT name, phone_number, license_plate
FROM atm_transactions AS atm
JOIN bank_accounts AS bank
ON atm.account_number = bank.account_number
JOIN people
ON bank.person_id = people.id
WHERE day = 28
AND month = 7
AND year = 2020
AND atm_location = "Fifer Street"
AND transaction_type = "withdraw";
-- Results:
-- name | phone_number | license_plate
-- Ernest | (367) 555-5533 | 94KL13X
-- Russell | (770) 555-1861 | 322W7JE
-- Roy | (122) 555-4581 | QX4YZN3
-- Bobby | (826) 555-1652 | 30G67EN
-- Elizabeth | (829) 555-5269 | L93JTIZ
-- Danielle | (389) 555-5198 | 4328GD8
-- Madison | (286) 555-6063 | 1106N58
-- Victoria | (338) 555-6650 | 8X428L0

-- Check for flights departing on 29th from Fiftyville
SELECT
filter.destination_airport_id,
people.name,
people.phone_number,
people.license_plate
FROM
    (SELECT *
    FROM flights
    JOIN airports
    ON airports.id = flights.origin_airport_id
    JOIN passengers
    ON flights.id = passengers.flight_id
    WHERE airports.city = "Fiftyville"
    AND flights.day = 29
    AND flights.month = 7
    AND flights.year = 2020
    AND flights.hour < 12
    ) AS filter
JOIN people
ON people.passport_number = filter.passport_number
ORDER BY filter.hour ASC;
-- Results
-- destination_airport_id | name | phone_number | license_plate
-- 4 | Doris | (066) 555-9701 | M51FA04
-- 4 | Roger | (130) 555-0289 | G412CB7
-- 4 | Ernest | (367) 555-5533 | 94KL13X
-- 4 | Edward | (328) 555-1152 | 130LD9Z
-- 4 | Evelyn | (499) 555-9472 | 0NTHK55
-- 4 | Madison | (286) 555-6063 | 1106N58
-- 4 | Bobby | (826) 555-1652 | 30G67EN
-- 4 | Danielle | (389) 555-5198 | 4328GD8
-- 1 | Daniel | (971) 555-6468 | FLFN3W0
-- 1 | Carol | (168) 555-6126 | 81MNC9R
-- 1 | Rebecca | (891) 555-5672 |
-- 1 | Sophia | (027) 555-1068 | 13FNH73
-- 1 | Heather | (502) 555-6712 |
-- 1 | Marilyn | (568) 555-3190 | 0R0FW39

-- Find all callers around minute of robbery
SELECT
theif.name AS caller_name,
calls.caller,
theif.license_plate AS caller_plate,
accomplice.name AS receiver_name,
calls.receiver,
accomplice.license_plate AS receiver_plate
FROM phone_calls AS calls
JOIN people as theif
ON calls.caller = theif.phone_number
JOIN people as accomplice
ON calls.receiver = accomplice.phone_number
WHERE calls.day = 28
AND calls.month = 7
AND calls.year = 2020
AND calls.duration <= 60;
-- Results:
-- caller_name | caller | caller_plate | receiver_name | receiver | receiver_plate
-- Roger | (130) 555-0289 | G412CB7 | Jack | (996) 555-8899 | 52R0Y8U
-- Evelyn | (499) 555-9472 | 0NTHK55 | Larry | (892) 555-8872 | O268ZZ0
-- Ernest | (367) 555-5533 | 94KL13X | Berthold | (375) 555-8161 | 4V16VO0
-- Kathryn | (609) 555-5876 | 4ZY7I8T | Danielle | (389) 555-5198 | 4328GD8
-- Evelyn | (499) 555-9472 | 0NTHK55 | Melissa | (717) 555-1342 |
-- Madison | (286) 555-6063 | 1106N58 | James | (676) 555-6554 | Q13SVG6
-- Russell | (770) 555-1861 | 322W7JE | Philip | (725) 555-3243 | GW362R6
-- Kimberly | (031) 555-6622 | Q12B3Z3 | Jacqueline | (910) 555-3251 | 43V0R5D
-- Bobby | (826) 555-1652 | 30G67EN | Doris | (066) 555-9701 | M51FA04
-- Victoria | (338) 555-6650 | 8X428L0 | Anna | (704) 555-2131 |

-- From the above analysis we can see some matches:
SELECT caller_name, receiver_name
FROM
(
    SELECT license_plate
    FROM courthouse_security_logs
    WHERE day = 28
    AND month = 7
    AND year = 2020
    AND hour = 10
    AND minute >= 15
    AND minute <= 25
    AND activity = "exit"
) AS logs
JOIN
(
    SELECT name, phone_number, license_plate
    FROM atm_transactions AS atm
    JOIN bank_accounts AS bank
    ON atm.account_number = bank.account_number
    JOIN people
    ON bank.person_id = people.id
    WHERE day = 28
    AND month = 7
    AND year = 2020
    AND atm_location = "Fifer Street"
    AND transaction_type = "withdraw"
) AS atm
ON logs.license_plate = atm.license_plate
JOIN
(
    SELECT
    filter.destination_airport_id,
    people.name,
    people.phone_number,
    people.license_plate
    FROM
        (SELECT *
        FROM flights
        JOIN airports
        ON airports.id = flights.origin_airport_id
        JOIN passengers
        ON flights.id = passengers.flight_id
        WHERE airports.city = "Fiftyville"
        AND flights.day = 29
        AND flights.month = 7
        AND flights.year = 2020
        AND flights.hour < 12
        ) AS filter
    JOIN people
    ON people.passport_number = filter.passport_number
) as plane
ON logs.license_plate = plane.license_plate
JOIN
(
    SELECT
    theif.name AS caller_name,
    calls.caller,
    theif.license_plate AS caller_plate,
    accomplice.name AS receiver_name,
    calls.receiver,
    accomplice.license_plate AS receiver_plate
    FROM phone_calls AS calls
    JOIN people as theif
    ON calls.caller = theif.phone_number
    JOIN people as accomplice
    ON calls.receiver = accomplice.phone_number
    WHERE calls.day = 28
    AND calls.month = 7
    AND calls.year = 2020
    AND calls.duration <= 60
) as phones
ON logs.license_plate = phones.caller_plate;
-- Results:
-- caller_name | receiver_name
-- Ernest | Berthold

-- Checking abvoe results we can see Ernest takes a flight to destination_id 4
SELECT full_name, city, abbreviation
FROM airports
WHERE id = 4;
-- Results
-- full_name | city | abbreviation
-- Heathrow Airport | London | LHR