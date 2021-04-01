import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

month_names = [
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
    ]

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        name = request.form.get("name")
        if not name:
            return render_template("error.html", message="Input missing name")
        month = request.form.get("month")
        if not month or "":
            return render_template("error.html", message="Input missing month")
        day = request.form.get("day")
        if not day or "":
            return render_template("error.html", message="Input missing day")

        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)
        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        valid_months = zip(range(1, 13), month_names)
        valid_days = range(1, 32)

        users = db.execute("SELECT * FROM birthdays")

        def double_digits(n):
            if int(n) < 10:
                n = "0" + str(n)
            return n

        for user in users:
            user["month"] = double_digits(user["month"])
            user["day"] = double_digits(user["day"])

        return render_template("index.html", users=users, valid_months=valid_months, valid_days=valid_days)

@app.route("/delete", methods=["POST"])
def delete():
    query = "DELETE FROM birthdays WHERE birthdays.name = ? AND birthdays.month = ? AND birthdays.day = ?"
    db.execute(query, request.form.get("name"), request.form.get("month"), request.form.get("day"))
    return redirect("/")
