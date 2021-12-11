from flask import Flask, render_template, session
from blueprint_query.blueprint_query import user_app
from edit_blueprint.edit import edit_app
from scenario_auth.routes import auth_app

import json


app = Flask(__name__)
app.register_blueprint(user_app, url_prefix='/queries')
app.config['SECRET_KEY'] = 'super secret key'
app.register_blueprint(auth_app, url_prefix='/authorize')
app.config['ACCESS_CONFIG'] = json.load(open('configs/assets.json'))
app.register_blueprint(edit_app, url_prefix='/edit')


@app.route('/')
def index():
    return render_template("main_menu.html")


@app.route('/session_clear')
def clear_session():
    session.clear()
    return render_template("main_menu.html")


if __name__ == '__main__':
    app.run(host="127.0.0.1", port=8000)
