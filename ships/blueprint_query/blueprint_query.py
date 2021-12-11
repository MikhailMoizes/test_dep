from flask import Blueprint
from flask import render_template, request
from sql_provider.sql_provider import SQLProvider
from database import work_with_db
from access import login_required, login_permission_required
import os

provider = SQLProvider(os.path.join(os.path.dirname(__file__), 'sql'))

db_config = {
    'host': '127.0.0.1',
    'port': 3306,
    'user': 'root',
    'password': '123qweasdzxc',
    'db': 'fly_tickets'
}
flag = 0
user_app = Blueprint('user_app', __name__, template_folder='templates', static_folder='static')

heads_1 = ['ID билета', 'Аэропорт вылета', 'Аэропорт прибытия', 'Дата вылета', 'Дата прибытия', 'Номер рейса']
heads_2 = ['Имя клиента', 'Номер рейса', 'Аэропорт вылета', 'Аэропорт прибытия', 'Дата прибытия', 'Номер рейса']
heads_3 = ['Дата рождения', 'Имя клиента', ' Номер рейса']


def outer(result):
    res = []
    for i in range(len(result)):
        res.append(list(result[i].values()))
    return res


@user_app.route('/query', methods=['GET', 'POST'])
@login_permission_required
def query():
    if request.method == 'GET':
        return render_template('menu_query.html', flag=0)
    else:
        select = request.form.get('queries', None)
        date = request.form.get('Date', None)
        airport = request.form.get('airport', None)
        pass_id = request.form.get('id', None)

        if select == '1' and date is None:
            return render_template("menu_query.html", flag=1, n=0)

        if select == '2' and airport is None:
            return render_template("menu_query.html", flag=2, n=0)

        if select == '3' and pass_id is None:
            return render_template("menu_query.html", flag=3, n=0)

        if date is not None and len(date) == 10:
            sql = provider.get('Aircraft.sql', dates=date)
            result = work_with_db(db_config, sql)
            if result:
                return render_template('menu_query.html', items=outer(result), n=len(outer(result)[0]), head=heads_1,
                                       flag=1)
            else:
                return render_template('menu_query.html', flag=4)
        elif airport is None and pass_id is None:
            return render_template("menu_query.html", flag=1, n=0)

        if airport is not None:
            sql = provider.get('airport.sql', airport=airport)
            result = work_with_db(db_config, sql)
            if result:
                return render_template('menu_query.html', items=outer(result), n=len(outer(result)[0]), head=heads_2,
                                       flag=2)
            else:
                return render_template('menu_query.html', flag=4)

        if pass_id is not None:
            sql = provider.get('clients_in_dates.sql', pass_id=pass_id)
            result = work_with_db(db_config, sql)
            if result:
                return render_template('menu_query.html', items=outer(result), n=len(outer(result)[0]), head=heads_3,
                                       flag=3)
            else:
                return render_template('menu_query.html', flag=4)
