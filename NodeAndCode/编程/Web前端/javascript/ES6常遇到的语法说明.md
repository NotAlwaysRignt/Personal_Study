##### export default
`export`与`export default`均可用于导出常量、函数、文件、模块等,只是用法有些区别。通过`export`方式导出，在导入时要加{ }，`export default`则不需要。
使用`export default`命令，为模块指定默认输出，这样就不需要知道所要加载模块的变量名,即在`import`时也可以自定义名字,不需要和`export default`的变量名相同,但`export`则一定要求名字一致

```js
var name="Bob";
export { name }
//import { name } from "/.a.js" 

var name="Bob";
export default name
//import name from "/.a.js" 这里 name 不需要大括号
//使用 export default 导出的变量,在import 时也可以自定义名字,即上面不一定要用 name
```

##### ES6 对象定义
以前`javascript`对象定义一定要用键值对,但是 ES6 支持`{对象,对象}`的形式,举例
```js
//以前
person = { name:"Bob",age:12 }

//ES6
name="Bob";
age = "12";
person = {name,age} //与 person = { name:"Bob",age:12 } 相同
```
