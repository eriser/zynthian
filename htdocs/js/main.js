/*
 * requestAnimationFrame pollyfill
 */
if (!window.requestAnimationFrame) {
	window.requestAnimationFrame = (window.webkitRequestAnimationFrame || window.mozRequestAnimationFrame || window.msRequestAnimationFrame || window.oRequestAnimationFrame || function (callback) {
		return window.setTimeout(callback, 1000 / 60);
	});
}

// Init Stats
var stats = new Stats();
stats.setMode(0);
stats.domElement.style.position = 'absolute';
stats.domElement.style.right = '0px';
stats.domElement.style.bottom = '0px';
//document.body.appendChild(stats.domElement);


//Calculate Constellation Params
var w = window.innerWidth;
var h = window.innerHeight;
//var length= Math.floor(Math.max(w, h)/10);
var length= Math.floor(w*h/5000);
//alert(length)

// Init plugin
$('canvas').constellation({
	line: {
		color: 'rgba(248, 207, 43, 0.6)'
	},
	length: length,
	radius: 200,
	velocity: 0.1,
	distance: 120
});
