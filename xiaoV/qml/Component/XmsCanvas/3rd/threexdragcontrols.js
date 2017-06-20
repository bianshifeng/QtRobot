//Qt.include("./three.js")

// _eventElement  : XmsControlEventSource
THREE.DragControls = function(_camera, _objects, _eventElement){
//    var _projector = new THREE.Projector();
    var _raycaster = new THREE.Raycaster();

    var _mouse = new THREE.Vector3();
    var _mouseOffset = new THREE.Vector3();

    var _selected, _hovered;

    var p3_sub_p1 = new THREE.Vector3();
    var targetPosition = new THREE.Vector3();
    var zeroVector = new THREE.Vector3();

    this.enabled = true;

    /* Custom Event Handing*/

    var _eventListeners ={

    };

    var me = this;
    //注册绑定在事件上的方法，一对多
    this.on = function(event,handler){
        if(!_eventListeners[event]){
            _eventListeners[event] =[];
        }

        _eventListeners[event].push(handler);
        return me;
    };
    //
    this.off = function(event,handler){
        var t_event_handlers = _eventListeners[event];
        if(!t_event_handlers) return me;

        if(t_event_handlers.indexOf(handler) > -1){
            t_event_handlers.splice(handler,1);
        }
        return me;
    }

    //给指定事件的处理方法传递参数
    var notify = function(event,data,member){
        var t_event_handlers =_eventListeners[event];
        if( !t_event_handlers) return;

        if( !member){
            for( var i = 0; i< t_event_handlers.length; i++){
                t_event_handlers[i](data);
            }
        }
    }


    this.setObjects = function(objects){
        if(objects instanceof THREE.Scene){  //直接将场景中的所有对象传入
            _objects = objects.children;
        } else {
            _objects = objects;
        }
    };



    this.activate = function(){
        _eventElement.addEventListener('mousemove',onDocumentMouseMove,false);
        _eventElement.addEventListener('mousedown',onDocumentMouseDown,false);
        _eventElement.addEventListener('mouseup',onDocumentMouseUp,false);
    };
    this.deactivate = function(){
        _eventElement.removeEventListener('mousemove',onDocumentMouseMove,false);
        _eventElement.removeEventListener('mousedown',onDocumentMouseDown,false);
        _eventElement.removeEventListener('mouseup',onDocumentMouseUp,false);

    };

    this.dispose = function(){
        me.deactivate();
    }

    function onDocumentMouseMove(event){
//        event.preventDefault();
        _mouse.x =(event.clientX/_eventElement.width) * 2 -1;
        _mouse.y = -(event.clientY/_eventElement.height) *2 +1;
        _raycaster.setFromCamera(_mouse,_camera);
        var t_ray = _raycaster.ray;



        // 1. 执行拖拽功能
        if(_selected && me.enabled){
            console.log(_selected.object.position.x);
            var t_normal = _selected.point;


            // I found this article useful about plane-line intersections
            // http://paulbourke.net/geometry/planeline/

            var t_denom = t_normal.dot(t_ray.direction);
            if(t_denom === 0){
                //bail
                console.log("no or infinite solutions");
                return;
            }

            var t_num = t_normal.dot(p3_sub_p1.copy(_selected.point).sub(t_ray.origin));
            var t_u = t_num/t_denom;

            targetPosition.copy(t_ray.direction).multiplyScalar(t_u).add(t_ray.origin).sub(_mouseOffset);
            //_selected.object.position.copy(targetPosition);

            var t_xLock, t_yLock, t_zLock = false;

            var t_moveX, t_moveY,t_moveZ;

            if(t_xLock){
                t_moveX = false;
                t_moveY = true;
                t_moveZ = true;
            }else if(t_yLock){
                t_moveX = true;
                t_moveY = false;
                t_moveZ = true;
            }else if(t_zLock) {
                t_moveX = true;
                t_moveY = true;
                t_moveZ = false;

            }else{
                t_moveX = t_moveY = t_moveZ = true;
            }

            //Reverse Matrix?
            if(t_moveX) _selected.object.position.x = targetPosition.x;
            if(t_moveY) _selected.object.position.y = targetPosition.y;
//            if(t_moveZ) _selected.object.position.z = targetPosition.z;

            notify('draging',_selected);

            return;
        }


        //2. 执行获取对象焦点功能


        var t_intersects = _raycaster.intersectObjects(_objects);
        if(t_intersects.length > 0){
//            _eventElement.style.cursor = 'pointer';
            _eventElement.styleCursor = Qt.PointingHandCursor;
            _hovered = t_intersects[0];
            notify('hoveron',_hovered);
        } else {
            notify('hoveroff',_hovered);
            _hovered = null;
            _eventElement.styleCursor = Qt.ArrowCursor   //'auto';
        }
    }


    function onDocumentMouseDown(event){
//        event.preventDefault();
        _mouse.x =(event.clientX/_eventElement.width) * 2 -1;
        _mouse.y = -(event.clientY/_eventElement.height) *2 +1;
        _raycaster.setFromCamera(_mouse,_camera);

        var t_interselects = _raycaster.intersectObjects(_objects);
        var t_ray = _raycaster.ray;
        var t_normal = t_ray.direction; // normal ray to the camera position

        if(t_interselects.length > 0){
            _selected = t_interselects[0];
            _selected.ray = t_ray;

            _mouseOffset.copy(_selected.point).sub(_selected.object.position);

            _eventElement.styleCursor = Qt.DragMoveCursor;
            notify('dragstart',_selected);
        }
    }


    function onDocumentMouseUp(event){
        //        event.preventDefault();
        if(_selected){
            notify('dragend',_selected);
            _selected = null;
        }

        _eventElement.styleCursor =Qt.ArrowCursor;// 'auto';

    }


    this.setObjects(_objects);
    this.activate();
}



