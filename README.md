# CSpeed #
	www.supjos.cn			All Rights Reserved.
------
**CSpeed** 是一个全堆栈的C语言PHP扩展, 基于MVC模式,专为高性能的API设计开发。

一个简单的 **示例** :
	
	/* 实例化扩展程序类 */
	$app = new supjos\mvc\App();
	
	/* 将当前目录设置为引入目录， 方法可以多次调用 */
	$app->setIncludeDirs('.');	
	
	/* 或者一次传入多个目录 */
	$app->setIncludeDirs([
		'.',
		'/home/cspeed/controllers',
		'/home/cspeed/models'
	]);
	
	/* 设置了引入目录后，以后的类CSpeed都能从以上目录自动加载 */

	/**
	 *  处理GET请求, 
	 *  第一个参数支持使用正则表达式进行URL匹配.
	 *  第二个参数表示一个匹配URL后进行的函数操作
	 *  第二个参数支持两种类型的参数: 匿名函数|实现 ```supjos\mvc\Callback```接口的类
	 */
	$app->get('/index/index$', function($controller){
		echo '<div style="width:200px;margin: 0 auto;"><h1>Hello CSpeed.</h1></div>';
	});
	
	/**
	 * 或者如下的形式,方便进行MVC三层开发
 	 */
 	  $app->get('/index', new class implements supjos\mvc\Callback(){
 	  	/** 
 	  	 * 接口仅包含一个方法 : init
 	  	 */
 	  	function init($controller){
 	  		/* 您可以在此方法里面将请求转发至其余的控制器方法 */
 	  		echo '<div style="width:200px;margin: 0 auto;"><h1>Hello CSpeed.</h1></div>';
 	  	}
 	  });
 	  
###IOC 注入容器类###
	
	$di = new supjos\tool\Di();
	
	$di->set('view', function(){
		/* 返回此注入类的实例 */
		return new supjos\mvc\View();
	});
	
	/* 当需要从容器中获取时 */
	$view = $di->get('view');
	
###View###
	
	/* 实例化视图类 */
	$view = new supjos\mvc\View();
	
	/* 操作视图进行渲染 */
	$view->render('index.phtml', ['version'=>'1.1.8', 'engine'=>'CSpeed']);
	
	/* 将变量或者内容输出到模版,可以重复多次调用 */
	$view->setVar('version', 'v12.1');
	
	/* 渲染视图 */
	$view->partial('test.phtml',['version'=>'v1.8.1']);
	
	/* 将视图内容进行获取而不输出 */
	$viewData = $view->getRender('test.phtml', ['version'=>'v1.8.1', 'engine'=>'CSpeed']);
	
###模型###
	
	/*下个版本考虑加入连接池功能*/

	$mysql = new supjos\mvc\MySql(
		[
			'dsn' 	 	=>		'mysql:host=localhost;dbname=cspeed',
			'username'	=>		'root',
			'password'	=>		'Root@localhost'
		]
	);
	
	/* 查询满足条件的所有数据 */
	$goods = $mysql->select(['id', 'price', 'good_name'])
				 ->from('www_good')
				 ->where(['price'=>'8.8])
				 ->orderBy('price DESC')
				 ->findAll();
	 
	/* 查询满足条件的一条数据 */
	$goods = $mysql->select(['id', 'price', 'good_name'])
				 ->from('www_good')
				 ->where(['price'=>'8.8])
				 ->orderBy('price DESC')
				 ->findOne();
				 
 	/* 原生SQL查询, 使用预处理绑定参数 */
	$goods = $mysql->query('SELECT * FROM www_good WHERE price >:price', [ ':price'=>'8.8' ]);
	
	/* 原生SQL语句更新或者删除 */
	$rows = $mysql->execute(' INSERT INTO www_good (id, price, good_name) VALUES ( :id, :price, :good_name ) ', [ ':id'=>'22', ':price'=>'89.8', ':good_name'=>'Apple' ]);
	
	$rows = $mysql->execute(' UPDATE www_good SET price=:price, good_name=:good_name WHERE id=:id ', [ ':id'=>'22', ':price'=>'89.8', ':good_name'=>'Apple' ]);
	
	/* 批量插入 */
	$data = [];
	for ($i = 0; $i < 10000; $i++){
		$data[] = [
			$i,
			$i+1,
			$i+2,
			$i+3
		];
	}
	$rows = $mysql->insert('www_product', ['id', 't_id', 'r_id', 'j_id], $data);
	
###HTTP-REQUEST | HTTP_RESPOSNE###
	
	/**
	 * HTTP REQEUST
	 */
	$request = new supjos\net\Request();
	
	/* 是否GET请求 */
	$request->isGet();
	
	/* 是否POST请求 */
	$request->isPost();
	
	/* 是否PUT请求 */
	$request->isPut();
	
	/* 是否DELETE请求 */
	$request->isDelete();
	
	/* 是否HEAD请求 */
	$request->isHead();
	
	/* 是否OPTIONS请求 */
	$request->isOptions();
	
	/* 获取$_GET */
	$request->get();
	
	/* 获取$_POST */
	$request->getPost();
	
	/* 获取QUERY_STRING */
	$request->getQuery();
	
	/*获取USER-AGENT */
	$request->getUserAgent();
	
	/* 获取ReqeustUri */
	$request->getRequestUri();
	
	
	/* HTTP Resposne */
	$response = new supjos\net\Response();
	
	/* 设置响应的JSON内容 */
	$response->setJsonData(['hello'=>'Speed']);
	
	/* 设置响应的原生内容 */
	$response->setRawData("Hello CSpeed");
	
	/* 渲染设置的内容 */
	$response->send();
	
	/* 设置响应HTTP头 */
	$response->setHeader($key, $value);


	
	
	






















	
