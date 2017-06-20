.pragma library

function XmsChart() {};

XmsChart.REVISION = '01';

//browserify support

if(typeof module === 'object'){
    module.exports = XmsChart;
}


XmsChart.log = function(){console.log.apply(console,arguments);}
XmsChart.warn = function(){console.warn.apply(console,arguments);}
XmsChart.error = function(){console.error.apply(console,arguments);}

XmsChart.MOUSE ={LEFT: Qt.LeftButton,MIDDLE: Qt.MiddleButton, RIGHT:Qt.RightButton};


