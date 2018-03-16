首先必须明确AOP编程中的几个概念,知道这些概念才不会被复杂的配置弄晕
join point 连接点  被拦截的一个 method
pointcut 切点,根据官网的定义,pointcut 可以理解为用于描述 join point 的表达式,它描述了哪些join point
         才是需要被拦截的
Advice   通知,在被拦截的 method 之前或之后要执行的 method, 即是我们要插入的函数, Advice 总是和 
         pointcut 成对组合在一起,只有确定了 Advice 和 poincut ,我们才知道要拦截哪些 joinpoint
Advisor ： Advice 和 Pointcut 组成的独立的单元,逻辑上是一个切面,切面最终被应用到目标函数(被拦截的函数)


在使用XML配置时就是遵循这几个概念的

第一种是 the name 方式 (指定拦截方法的具体名字)

    被拦截的对象一个bean,设为beanA
    我们要用于织入的方法,写在一个类中,作为一个bean,设为beanB

    我们往往不是拦截一个对象的所有方法,而是具体指定该类的某个特定的方法,写成一个bean  (Pointcut)
    将 要织入的方法的bean 和 Pointcut bean 组装成一个 Advisor bean(切面)
    再将 要织入的方法类的bean(beanB) 和 AdvisorBean 组装成一个 ProxyFactory bean 相当于把切面织入

    在获取对象时,我们要获取 ProxyFactory bean 所指的对象,这个对象就是我们最终所需要的结果

第二种是使用正则表达式方式 (不用被拦截方法的具体名字,而是采用正则表达式的方式拦截一类方法)

    被拦截的对象一个bean, 设为beanA
    我们要用于织入的方法,写在一个类中,作为一个bean, 设为beanB

    将 要织入的方法的bean 和 被拦截方法的正则表达式 组装成一个 Advisor bean(切面)
    再将 要织入的方法类的bean(beanB) 和 AdvisorBean 组装成一个 ProxyFactory bean 相当于把切面织入

    在获取对象时,我们要获取 ProxyFactory bean 所指的对象,这个对象就是我们最终所需要的结果


