#include "../Epaper/epaper.h"
#include "wifi_html.h"
#if(EINK_UI_LANGUAGE == EINK_LANGUAGE_CHINESE)
/******** HTML Web page code  *********************/
extern const char page_html_config[] ICACHE_RODATA_ATTR = "<html lang = 'en'>\r\n\
<head>\r\n\
<meta charset = 'UTF-8'>\r\n\
<meta name='viewport' content='width=device-width,initial-scale=1.0'>\r\n\
<title>Document</title>\r\n\
<style>\r\n\
body {\r\n\
background-color:rgb(221,221,221);\r\n\
}\r\n\
table {\r\n\
width: 350px;\r\n\
height: 700px;\r\n\
margin: 10px auto;\r\n\
background-color: rgb(255, 255, 255);\r\n\
text-align:center;\r\n\
border: 1px solid rgb(0, 0, 0);\r\n\
border-radius: 30px;\r\n\
}\r\n\
input {\r\n\
width: 200px;\r\n\
height: 25px;\r\n\
line-height: 25px;\r\n\
border-radius: 25px;\r\n\
text-align:center;\r\n\
background-color: rgb(232,239,255);\r\n\
border:rgb(255, 255,255);\r\n\
}\r\n\
span input {\r\n\
width: 100px;\r\n\
height: 30 px;\r\n\
color: #fff;\r\n\
line-height: 25px;\r\n\
background-color: rgb(103,124,333);\r\n\
}\r\n\
</style>\r\n\
</head>\r\n\
<body>\r\n\
<table>\r\n\
<tr>\r\n\
<td>\r\n\
<font size='5'><b>彩虹天气初装设置</b></font><br>\r\n\
<hr align='center' width='300px' size='2px'><br>\r\n\
<form name='input' action='/' method='POST'>\r\n\
<input type= 'text' name='ssid' placeholder='请输入[2.4G]WIFI名称'     style='background-color:rgb(242,185,231);'><br><br>\r\n\
<input type= 'text' name='password' placeholder='请输入[2.4G]WIFI密码' style='background-color:rgb(217,186,240);'><br><br>\r\n\
<input type= 'text' name='key' placeholder='请输入心知天气私钥' style='background-color:rgb(211, 116, 152);'><br><br>\r\n\
<input type= 'number' name='locaew' min='-180.00' max = '180.00' step = '0.000001' placeholder='请输入经度(东经+西经-)'       style='background-color:rgb(253,156,108);'><br><br>\r\n\
<input type= 'number' name='locasn' min='-90.00' max = '90.00' step = '0.000001' placeholder='请输入纬度(北纬+南纬-)'       style='background-color:rgb(253,156,108);'><br><br>\r\n\
<input type= 'text' name='height' placeholder='请输入当地海拔' style='background-color:rgb(242,241,187);'><br><br>\r\n\
<input type= 'number' name='windir' min='0.00' max = '360.00' step = '0.000001' placeholder='请输入您的窗子朝向'     style='background-color:rgb(185,241,206);'><br><br>\r\n\
<span>\r\n\
<input type='reset' value= '重置'>\r\n\
<input type='submit' value= '连接'>\r\n\
</span>\r\n\
</form>\r\n\
<canvas id='test' width='340' height='160'></canvas>\r\n\
<script >\r\n\
var cas = document.getElementById('test')\r\n\
var ctx = cas.getContext('2d')\r\n\
ctx.lineWidth='8'\r\n\
ctx.lineCap='round'\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(144,24,207)'\r\n\
ctx.arc(138,150, 80, 1.83*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(0,140,253)'\r\n\
ctx.arc(138,150, 88, 1.83*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(2,87,157)'\r\n\
ctx.arc(138,150, 96, 1.85*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(3,142,75)'\r\n\
ctx.arc(138,150, 104, 1.86*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(254,250,0)'\r\n\
ctx.arc(138,150, 112, 1.88*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(253,169,4)'\r\n\
ctx.arc(138,150, 120, 1.91*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(227,68,6)'\r\n\
ctx.arc(138,150, 128, 1.90*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.lineWidth='4'\r\n\
ctx.lineCap='square'\r\n\
ctx.strokeStyle='rgb(0,0,0)'\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(83,150)\r\n\
ctx.lineTo(83,114)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(101,132, 18, 1.85*Math.PI,1.15*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(125,114)\r\n\
ctx.lineTo(143,114)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(161,132)\r\n\
ctx.lineTo(161,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(143,132, 18, 0, 2*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(167,114)\r\n\
ctx.lineTo(167,116)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(167,122)\r\n\
ctx.lineTo(167,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(173,114)\r\n\
ctx.lineTo(173,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(191,132, 18, 0, 1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(209,132)\r\n\
ctx.lineTo(209,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(215,114)\r\n\
ctx.lineTo(215,132)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(233,132, 18, 0, 2*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(275,132, 18, 0, 2*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(299,114)\r\n\
ctx.lineTo(299,142)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(317,114)\r\n\
ctx.lineTo(317,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(335,114)\r\n\
ctx.lineTo(335,142)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(307,150)\r\n\
ctx.lineTo(327,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(307,142, 8, 1*Math.PI, 0.5*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(327,142, 8, 0, 0.5*Math.PI, false)\r\n\
ctx.stroke()\r\n\
</script> <br>\r\n\
<font size='2'>连接wifi仅用于授时,不收集任何个人隐私</font><br>\r\n\
<font size='2'>B站 歪思whys 专利出品 商业仿冒必究</font><br>\r\n\
</td>\r\n\
</tr>\r\n\
</table>\r\n\
</body>\r\n\
";

extern const char page_html_check[] ICACHE_RODATA_ATTR = "<html lang = 'en'>\r\n\
<head>\r\n\
<meta charset = 'UTF-8'>\r\n\
<meta name='viewport' content='width=device-width,initial-scale=1.0'>\r\n\
<title>Document</title>\r\n\
<style>\r\n\
body {\r\n\
background-color:rgb(221,221,221);\r\n\
}\r\n\
table {\r\n\
width: 350px;\r\n\
height: 700px;\r\n\
margin: 10px auto;\r\n\
background-color: rgb(255, 255, 255);\r\n\
text-align:center;\r\n\
border: 1px solid rgb(0, 0, 0);\r\n\
border-radius: 30px;\r\n\
}\r\n\
input {\r\n\
width: 200px;\r\n\
height: 30px;\r\n\
line-height: 30px;\r\n\
border-radius: 30px;\r\n\
text-align:center;\r\n\
background-color: rgb(232,239,255);\r\n\
border:rgb(255, 255,255);\r\n\
}\r\n\
span input {\r\n\
width: 100px;\r\n\
height: 40 px;\r\n\
color: #fff;\r\n\
line-height: 30px;\r\n\
background-color: rgb(103,124,333);\r\n\
}\r\n\
</style>\r\n\
</head>\r\n\
<body>\r\n\
<table>\r\n\
<tr>\r\n\
<td>\r\n\
<font size='5'><b>恭喜！设置成功</b></font><br>\r\n\
<font size='5'><b>设备将重启到终端模式</b></font><br>\r\n\
<font size='5'><b>现在可关闭该页面了</b></font><br>\r\n\
<br> <br>\r\n\
<canvas id='test' width='340' height='160'></canvas>\r\n\
<script >\r\n\
var cas = document.getElementById('test')\r\n\
var ctx = cas.getContext('2d')\r\n\
ctx.lineWidth='8'\r\n\
ctx.lineCap='round'\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(144,24,207)'\r\n\
ctx.arc(138,150, 80, 1.83*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(0,140,253)'\r\n\
ctx.arc(138,150, 88, 1.83*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(2,87,157)'\r\n\
ctx.arc(138,150, 96, 1.85*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(3,142,75)'\r\n\
ctx.arc(138,150, 104, 1.86*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(254,250,0)'\r\n\
ctx.arc(138,150, 112, 1.88*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(253,169,4)'\r\n\
ctx.arc(138,150, 120, 1.91*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(227,68,6)'\r\n\
ctx.arc(138,150, 128, 1.90*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.lineWidth='4'\r\n\
ctx.lineCap='square'\r\n\
ctx.strokeStyle='rgb(0,0,0)'\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(83,150)\r\n\
ctx.lineTo(83,114)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(101,132, 18, 1.85*Math.PI,1.15*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(125,114)\r\n\
ctx.lineTo(143,114)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(161,132)\r\n\
ctx.lineTo(161,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(143,132, 18, 0, 2*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(167,114)\r\n\
ctx.lineTo(167,116)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(167,122)\r\n\
ctx.lineTo(167,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(173,114)\r\n\
ctx.lineTo(173,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(191,132, 18, 0, 1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(209,132)\r\n\
ctx.lineTo(209,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(215,114)\r\n\
ctx.lineTo(215,132)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(233,132, 18, 0, 2*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(275,132, 18, 0, 2*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(299,114)\r\n\
ctx.lineTo(299,142)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(317,114)\r\n\
ctx.lineTo(317,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(335,114)\r\n\
ctx.lineTo(335,142)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(307,150)\r\n\
ctx.lineTo(327,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(307,142, 8, 1*Math.PI, 0.5*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(327,142, 8, 0, 0.5*Math.PI, false)\r\n\
ctx.stroke()\r\n\
</script> <br>\r\n\
<font size='2'>连接wifi仅用于授时,不收集任何个人隐私</font><br>\r\n\
<font size='2'>B站 歪思whys 专利出品 商业仿冒必究</font><br>\r\n\
</td>\r\n\
</tr>\r\n\
</table>\r\n\
</body>\r\n\
";

extern const char page_html_error[] ICACHE_RODATA_ATTR = "<html lang = 'en'>\r\n\
<head>\r\n\
<meta charset = 'UTF-8'>\r\n\
<meta name='viewport' content='width=device-width,initial-scale=1.0'>\r\n\
<title>Document</title>\r\n\
<style>\r\n\
body {\r\n\
background-color:rgb(221,221,221);\r\n\
}\r\n\
table {\r\n\
width: 350px;\r\n\
height: 700px;\r\n\
margin: 10px auto;\r\n\
background-color: rgb(255, 255, 255);\r\n\
text-align:center;\r\n\
border: 1px solid rgb(0, 0, 0);\r\n\
border-radius: 30px;\r\n\
}\r\n\
input {\r\n\
width: 200px;\r\n\
height: 30px;\r\n\
line-height: 30px;\r\n\
border-radius: 30px;\r\n\
text-align:center;\r\n\
background-color: rgb(232,239,255);\r\n\
border:rgb(255, 255,255);\r\n\
}\r\n\
span input {\r\n\
width: 100px;\r\n\
height: 40 px;\r\n\
color: #fff;\r\n\
line-height: 30px;\r\n\
background-color: rgb(103,124,333);\r\n\
}\r\n\
</style>\r\n\
</head>\r\n\
<body>\r\n\
<table>\r\n\
<tr>\r\n\
<td>\r\n\
<font size='5'><b>抱歉！参数错误</b></font><br>\r\n\
<font size='5'><b>请打开192.168.4.1页面重试</b></font><br>\r\n\
<br> <br>\r\n\
<canvas id='test' width='340' height='160'></canvas>\r\n\
<script >\r\n\
var cas = document.getElementById('test')\r\n\
var ctx = cas.getContext('2d')\r\n\
ctx.lineWidth='8'\r\n\
ctx.lineCap='round'\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(144,24,207)'\r\n\
ctx.arc(138,150, 80, 1.83*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(0,140,253)'\r\n\
ctx.arc(138,150, 88, 1.83*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(2,87,157)'\r\n\
ctx.arc(138,150, 96, 1.85*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(3,142,75)'\r\n\
ctx.arc(138,150, 104, 1.86*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(254,250,0)'\r\n\
ctx.arc(138,150, 112, 1.88*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(253,169,4)'\r\n\
ctx.arc(138,150, 120, 1.91*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(227,68,6)'\r\n\
ctx.arc(138,150, 128, 1.90*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.lineWidth='4'\r\n\
ctx.lineCap='square'\r\n\
ctx.strokeStyle='rgb(0,0,0)'\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(83,150)\r\n\
ctx.lineTo(83,114)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(101,132, 18, 1.85*Math.PI,1.15*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(125,114)\r\n\
ctx.lineTo(143,114)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(161,132)\r\n\
ctx.lineTo(161,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(143,132, 18, 0, 2*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(167,114)\r\n\
ctx.lineTo(167,116)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(167,122)\r\n\
ctx.lineTo(167,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(173,114)\r\n\
ctx.lineTo(173,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(191,132, 18, 0, 1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(209,132)\r\n\
ctx.lineTo(209,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(215,114)\r\n\
ctx.lineTo(215,132)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(233,132, 18, 0, 2*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(275,132, 18, 0, 2*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(299,114)\r\n\
ctx.lineTo(299,142)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(317,114)\r\n\
ctx.lineTo(317,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(335,114)\r\n\
ctx.lineTo(335,142)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(307,150)\r\n\
ctx.lineTo(327,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(307,142, 8, 1*Math.PI, 0.5*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(327,142, 8, 0, 0.5*Math.PI, false)\r\n\
ctx.stroke()\r\n\
</script> <br>\r\n\
<font size='2'>连接wifi仅用于授时,不收集任何个人隐私</font><br>\r\n\
<font size='2'>B站 歪思whys 专利出品 商业仿冒必究</font><br>\r\n\
</td>\r\n\
</tr>\r\n\
</table>\r\n\
</body>\r\n\
";

extern const char page_html_oldconfig[] ICACHE_RODATA_ATTR = "<html lang = 'en'>\r\n\
<head>\r\n\
<meta charset = 'UTF-8'>\r\n\
<meta name='viewport' content='width=device-width,initial-scale=1.0'>\r\n\
<title>Document</title>\r\n\
<style>\r\n\
body {\r\n\
background-color:rgb(221,221,221);\r\n\
}\r\n\
table {\r\n\
width: 350px;\r\n\
height: 700px;\r\n\
margin: 10px auto;\r\n\
background-color: rgb(255, 255, 255);\r\n\
text-align:center;\r\n\
border: 1px solid rgb(0, 0, 0);\r\n\
border-radius: 30px;\r\n\
}\r\n\
input {\r\n\
width: 50px;\r\n\
height: 30px;\r\n\
line-height: 30px;\r\n\
border-radius: 30px;\r\n\
text-align:center;\r\n\
background-color: rgb(232,239,255);\r\n\
border:rgb(255, 255,255);\r\n\
}\r\n\
span input {\r\n\
width: 150px;\r\n\
height: 30 px;\r\n\
color: #fff;\r\n\
line-height: 30px;\r\n\
background-color: rgb(103,124,333);\r\n\
}\r\n\
</style>\r\n\
</head>\r\n\
<body>\r\n\
<table>\r\n\
<tr>\r\n\
<td>\r\n\
<font size='5'><b>Flash中发现以前的设置</b></font><br>\r\n\
<font size='5'><b>是否继续使用它？</b></font><br>\r\n\
</br></br></br></br>\r\n\
<form name='input' action='/' method='POST'>\r\n\
<input type='radio' name='sel' value='yes' id='girl'> <font size='7'><b>是</b></font><br>\r\n\
<input type='radio' name='sel' value='no' id='boy' checked='checked'><font size='7'><b>否</b></font><br>\r\n\
</br></br>\r\n\
<span>\r\n\
<input type='submit' value= '确定'>\r\n\
</span>\r\n\
</form>\r\n\
</br></br></br></br>\r\n\
<canvas id='test' width='340' height='160'></canvas>\r\n\
<script >\r\n\
var cas = document.getElementById('test')\r\n\
var ctx = cas.getContext('2d')\r\n\
ctx.lineWidth='8'\r\n\
ctx.lineCap='round'\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(144,24,207)'\r\n\
ctx.arc(138,150, 80, 1.83*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(0,140,253)'\r\n\
ctx.arc(138,150, 88, 1.83*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(2,87,157)'\r\n\
ctx.arc(138,150, 96, 1.85*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(3,142,75)'\r\n\
ctx.arc(138,150, 104, 1.86*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(254,250,0)'\r\n\
ctx.arc(138,150, 112, 1.88*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(253,169,4)'\r\n\
ctx.arc(138,150, 120, 1.91*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.strokeStyle='rgb(227,68,6)'\r\n\
ctx.arc(138,150, 128, 1.90*Math.PI,1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.lineWidth='4'\r\n\
ctx.lineCap='square'\r\n\
ctx.strokeStyle='rgb(0,0,0)'\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(83,150)\r\n\
ctx.lineTo(83,114)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(101,132, 18, 1.85*Math.PI,1.15*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(125,114)\r\n\
ctx.lineTo(143,114)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(161,132)\r\n\
ctx.lineTo(161,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(143,132, 18, 0, 2*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(167,114)\r\n\
ctx.lineTo(167,116)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(167,122)\r\n\
ctx.lineTo(167,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(173,114)\r\n\
ctx.lineTo(173,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(191,132, 18, 0, 1*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(209,132)\r\n\
ctx.lineTo(209,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(215,114)\r\n\
ctx.lineTo(215,132)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(233,132, 18, 0, 2*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(275,132, 18, 0, 2*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(299,114)\r\n\
ctx.lineTo(299,142)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(317,114)\r\n\
ctx.lineTo(317,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(335,114)\r\n\
ctx.lineTo(335,142)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.moveTo(307,150)\r\n\
ctx.lineTo(327,150)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(307,142, 8, 1*Math.PI, 0.5*Math.PI,true)\r\n\
ctx.stroke()\r\n\
ctx.beginPath()\r\n\
ctx.arc(327,142, 8, 0, 0.5*Math.PI, false)\r\n\
ctx.stroke()\r\n\
</script> <br>\r\n\
<font size='2'>连接wifi仅用于授时,不收集任何个人隐私</font><br>\r\n\
<font size='2'>B站 歪思whys 专利出品 商业仿冒必究</font><br>\r\n\
</td>\r\n\
</tr>\r\n\
</table>\r\n\
</body>\r\n\
";
#endif
