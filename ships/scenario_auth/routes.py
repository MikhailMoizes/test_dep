from flask import Blueprint, render_template, session, request, redirect
from database import compare

auth_app = Blueprint('auth', __name__, template_folder='templates', static_folder='static')


@auth_app.route('/', methods=['GET', 'POST'])
def login_page():
    if request.method == 'GET':
        return render_template('login.html')
    else:
        login = request.form.get('login', '')
        password = request.form.get('password', '')
        if compare(login, password):
            session['group_name'] = compare(login, password)
            return redirect(f"/")
        else:
            return render_template('login.html', text='Неверный логин или пароль')
