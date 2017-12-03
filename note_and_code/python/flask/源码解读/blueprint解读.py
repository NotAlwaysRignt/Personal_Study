#coding:utf-8
'''
先看看blueprint是如何注册的
设在app/main/__init__.py文件中,有:
from flask import Blueprints
blue = Blueprint('main',__name__)
则该蓝本对应的视图函数可以这样定义
@blue.route('/...')
def ... #视图函数


则可用 app.register_blueprint(blue) 注册这个蓝本
'''


先看Blueprint这个类,在flask 的 blueprints.py文件中
详细的就不展开了,会发现Blueprint这个类的很多方法与Flask类中的方法名称相同作用相似

def register_blueprint(self, blueprint, **options):
    first_registration = False
    if blueprint.name in self.blueprints: #蓝本的名字不能冲突,多个蓝本不可重名
        assert self.blueprints[blueprint.name] is blueprint, \
           
            (blueprint, self.blueprints[blueprint.name], blueprint.name)
    else:
        self.blueprints[blueprint.name] = blueprint
        self._blueprint_order.append(blueprint)
        first_registration = True
    blueprint.register(self, options, first_registration)
    
只看 blueprint.register  ,这时Blueprint类的方法(blueprints.py文件中)
def register(self, app, options, first_registration=False):

    self._got_registered_once = True
    state = self.make_setup_state(app, options, first_registration)
    if self.has_static_folder:
        state.add_url_rule(self.static_url_path + '/<path:filename>',
                           view_func=self.send_static_file,
                           endpoint='static')

    for deferred in self.deferred_functions:
        deferred(state)

