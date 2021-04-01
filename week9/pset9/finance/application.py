"""
Implements a profit and loss count on index page as an additional feature
"""

import os
from datetime import datetime

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

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    print(session["user_id"])
    holdings = db.execute("SELECT * FROM holdings WHERE id = ?", session["user_id"])
    cash = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    if not holdings:
        holdings = []

    data = []
    total = cash
    total_pandl = 0
    for holding in holdings:
        price = lookup(holding["symbol"])["price"]
        value = price * holding["units"]
        pandl = value - holding["cost"]

        total += value
        total_pandl += pandl
        data.append({
            "symbol": holding["symbol"],
            "units": holding["units"],
            "price": price,
            "value": value,
            "cost": holding["cost"],
            "pandl": pandl
        })

    return render_template("index.html", data=data, cash=cash, total=total, total_pandl=total_pandl)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 400)

        # Ensure number of shares was submitted
        elif not shares:
            return apology("must provide number of shares", 400)

        if not shares.isdigit():
            if not isinstance(shares, int):
                return apology("must be whole number of shares", 400)
        else:
            shares = int(shares)

        quote = lookup(symbol)
        if not quote:
            return apology("invalid ticker symbol", 400)

        cash = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        total = quote["price"] * shares

        if cash < total:
            return apology("you ain't got enough $", 400)
        else:
            updated_cash = cash - total
            db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, session["user_id"])
            db.execute("INSERT INTO orders (id, symbol, type, units, price, time) VALUES(?, ?, ?, ?, ?, ?)", session["user_id"], symbol, "BUY", shares, quote["price"], datetime.now())

        user_holding = db.execute("SELECT * FROM holdings WHERE id = ? AND symbol = ?", session["user_id"], symbol)
        if not user_holding:
            db.execute("INSERT INTO holdings (id, symbol, units, cost) VALUES(?, ?, ?, ?)", session["user_id"], symbol, shares, total)
        else:
            units = user_holding[0]["units"] + shares
            cost = user_holding[0]["cost"] + total
            db.execute("UPDATE holdings SET units = ?, cost = ? WHERE id = ? AND symbol = ?", units, cost, session["user_id"], symbol)

        return render_template("bought.html", quote=quote, shares=shares, total=total, cash=updated_cash)

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    orders = db.execute("SELECT * FROM orders WHERE id = ?", session["user_id"])

    if not orders:
        return apology("you haven't made any trades", 404)

    return render_template("history.html", orders=orders[::-1])


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        symbol = request.form.get("symbol")

        if not symbol:
            return apology("must provide symbol", 400)

        quote = lookup(symbol)
        if not quote:
            return apology("invalid ticker symbol", 400)

        return render_template("quoted.html", quote=quote, symbol=symbol)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure username is unique
        present = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if present:
            return apology("username is already taken", 400)

        # Ensure passwords match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        db.execute("INSERT INTO users (username, hash, cash) VALUES(?, ?, ?)", request.form.get("username"), generate_password_hash(request.form.get("password")), 10000)

        user_id = db.execute("SELECT id FROM users WHERE username = ?", request.form.get("username"))
        session["user_id"] = user_id[0]["id"]

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        symbol = request.form.get("symbol")
        units = int(request.form.get("shares"))

        # Ensure username was submitted
        if not symbol:
            return apology("must provide stock", 403)

        # Ensure password was submitted
        elif not units:
            return apology("must provide number of shares", 403)

        user_holding = db.execute("SELECT * FROM holdings WHERE id = ? AND symbol = ?", session["user_id"], symbol)
        if units > user_holding[0]["units"]:
            return apology("invalid number of shares", 400)

        quote = lookup(symbol)
        total = quote["price"] * units

        cash = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        updated_cash = cash + total

        new_cost = user_holding[0]["cost"] - total
        if new_cost < 0.0:
            new_cost = 0.0

        new_units = user_holding[0]["units"] - units
        if new_units == 0:
            db.execute("DELETE FROM holdings WHERE id = ? AND symbol = ?", session["user_id"], symbol)
        else:
            db.execute("UPDATE holdings SET units = ?, cost = ? WHERE id = ? AND symbol = ?", new_units, new_cost, session["user_id"], symbol)

        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, session["user_id"])
        db.execute("INSERT INTO orders (id, symbol, type, units, price, time) VALUES(?, ?, ?, ?, ?, ?)", session["user_id"], symbol, "SELL", units, quote["price"], datetime.now())
        return render_template("sold.html", quote=quote, shares=units, cash=updated_cash, total=total)

    else:
        holdings = db.execute("SELECT * FROM holdings WHERE id = ?", session["user_id"])

        if not holdings:
            return apology("you have nothing to sell", 400)

        return render_template("sell.html", holdings=holdings)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
