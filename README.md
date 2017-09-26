# CSpeed #

----------

## Simple Example ##

	$app = new supjos\mvc\App();
	
	$app->get('/index$', function($controller){
		
		echo '<div style="text-align:center;"><h1>Hello Speed</h1></div>';
	})

or

	$app->get('/index$', new class extends supjos\mvc\Callback{
		
		function init($controller) {
	
			echo 'This is a method come from the /index PATH_INFO.';
		}
	})