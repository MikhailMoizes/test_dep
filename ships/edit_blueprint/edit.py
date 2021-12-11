from flask import Blueprint
from flask import render_template, request
from edit_blueprint.sql_provider.sql_provider import SQLProvider
from database import work_with_db, db_update
from access import login_permission_required
import os

db_config = {
    'host': '127.0.0.1',
    'port': 3306,
    'user': 'root',
    'password': '123qweasdzxc',
    'db': 'fly_tickets'
}
provider = SQLProvider(os.path.join(os.path.dirname(__file__), 'sql'))
edit_app = Blueprint('edit_app', __name__, template_folder='templates', static_folder='static')

heads_1 = ['Имя клиента', 'Дата рождения', 'Номер документа', 'Номер рейса']
heads_2 = ['ID билета', 'Аэропорт отправления', 'Аэропорт прибытия', 'Дата отправления', 'Дата прибытия', 'Номер рейса']


def outer(result):
    res = []
    for i in range(len(result)):
        res.append(list(result[i].values()))
    return res


def in_result(id_ticket, result):
    for i in result:
        if id_ticket == i:
            return True


@edit_app.route('/', methods=['GET', 'POST'])
@login_permission_required
def index_delete():
    if request.method == 'POST':
        id_example = request.form.get('id_order', None)
        if id_example is not None:
            sql = provider.get('delete.sql', gen1=id_example)
            db_update(db_config, sql)
    sql = provider.get('all_order.sql')
    result = work_with_db(db_config, sql)
    return render_template('main.html', items=result, heads=heads_1)


@edit_app.route('/insert', methods=['GET', 'POST'])
@login_permission_required
def insert():
    if request.method == 'GET':
        sql = provider.get('all.sql')
        result = work_with_db(db_config, sql)
        return render_template('insert.html', items=outer(result), head=heads_2, n=len(result[0]))

    else:
        client_name = request.form.get('client_name', None)
        client_brth = request.form.get('client_brth', None)
        client_pass_id = request.form.get('client_pass_id', None)
        tickets_id = request.form.get('tickets_id', None)

        if client_name is not None and client_brth is not None and \
                client_pass_id is not None and tickets_id is not None:

            sql = provider.get('insert.sql', gen1=client_name, gen2=client_brth, gen3=client_pass_id, gen4=tickets_id)
            sql_1 = provider.get('insertion_delete.sql', gen4=tickets_id)
            db_update(db_config, sql)
            db_update(db_config, sql_1)
        return index_delete()
