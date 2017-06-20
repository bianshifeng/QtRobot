

function OrbitConstraint(camera){

    this.camera = camera;

    //"target" sets the location of focus,
    // where the camera oribits around and
    // where it pans with respect to.
    this.target = new THREE.Vector3();

    // Limits to how far you can dolly in and out (PerspectiveCamera only)

    this.minDistance = 0;
    this.maxDistance = Infinity;

    // Limits to how far you can zoom in and out(OrthographicCamera only)
    this.minZoom =0;
    this.maxZoom = Infinity;

    // How far you can orbit vertically, upper and lower limits;
    // Range is 0 to Math.PI radians.

    this.minPolarAngle = 0; //radians
    this.maxPolarAngle = Math.PI; // radians

    // How far you can orbit horizontally, upper and lower limits.
    // If set, must be a sub-interval of the interval [- Math.PI, Math.PI];
    this.minAzimuthAngle = - Infinity; // radians
    this.maxAzimuthAngle = Infinity; //radians

    // Set to true to enable damping(inertia)
    // If damping is enabled, you must call controls.update() in you update loop
    this.enableDamping = false;
    this.dampingFactor = 0.25;

    // internals
    var me = this;
    var EPS = 0.000001;

    // Current position in spherical coordinate systm.
    var theta;
    var phi;
    // Pending changes
    var phiDelta = 0;
    var thetaDelta =0;
    var scale = 1;
    var panOffset = new THREE.Vector3();
    var zoomChanged = false;

    //API

    this.getPolarAngle = function(){
        return phi;
    }
    this.getAzimuthalAngle = function(){
        return theta;
    }

    this.rotateLeft = function(angle){
        thetaDelta -= angle;
    }

    this.rotateUp = function(angle){
        phiDelta -= angle;
    }

    // pass in distance in world space to move left
    this.panLeft = function(){
        var v = new THREE.Vector3();
        return function planLeft(distance){
            var te = this.camera.matrix.elements;
            v.set(te[0],te[1],te[2]);
            v.multiplyScalar(-distance);
            panOffset.add(v);
        };
    }();

    // pass in distance in world space to move up

    this.panUp = function(){
        var v = new THREE.Vector3();
        return function panUp(distance){
            var te = this.camera.matrix.elements;
            // get Y column of matrix
            v.set(te[4],te[5],te[6]);
            v.multiplyScalar(distance);
            panOffset.add(v);
        };

    }();

    // pass in x,y of change desired in pixel space.
    // right and down are positive

    this.pan = function(deltaX, deltaY, screenWidth,screenHeight){
        if(this.camera instanceof THREE.PerspectiveCamera){
            // perspective
            var position = this.camera.position;
            var offset = position.clone().sub(this.target);
            var targetDistance = offset.length();

            // half of the fov is center to top of screen
            targetDistance *= Math.tan((me.camera.fov /2)* Math.PI/ 180.0);

            //we actually don't use screenWith, since
            // perspective camera is fixed to screen height
            this.panLeft(2*deltaX * targetDistance/screenHeight);
            this.panUp(2*deltaY*targetDistance/screenHeight);

        }else if(this.camera instanceof THREE.OrthographicCamera){
            //orthographic
            this.panLeft(deltaX*(this.camera.right - this.camera.left)/screenWidth);
            this.panUp(deltaY * (this.camera.top - this.camera.bottom)/screenHeight);
        }else{
            // camera neither orthographic or perspective
            console.warn('WARNING: OrbitControls.js encountered an unknown camera type -pan disabled.');

        }
    };


    this.dollyIn = function(dollyScale){
        dollyScale = Math.pow(0.95, dollyScale);
        if(this.camera instanceof THREE.PerspectiveCamera){
            scale /= dollyScale;
        }else if(this.camera instanceof THREE.OrthographicCamera){
            this.camera.zoom = Math.max(this.minZoom,
                                         Math.min(this.maxZoom,this.camera.zoom*dollyScale));
            this.camera.updateProjectionMatrix();
            zoomChanged = true;
        }else{
            console.warn('WARING: OrbitControls.js encountered an unknown camera type - dolly/zoom disabled.');
        }
    };


    this.dollyOut = function(dollyScale){

        dollyScale = Math.pow(0.95, dollyScale);

        if(this.camera instanceof THREE.PerspectiveCamera){
            scale *= dollyScale;

        }else if(this.camera instanceof THREE.OrthographicCamera){
            this.camera.zoom = Math.max(this.minZoom, Math.min(this.maxZoom,
                                                                this.camera.zoom/dollyScale));
            this.camera.updateProjectionMatrix();
            zoomChanged = true;
        }else{
            console.warn('WARING: OrbitControls.js encountered an unknown camera type - dolly/zoom disabled.');
        }
    }

    this.update = function(){
        var offset = new THREE.Vector3();
        // 1.为了旋转 so camera.up is the orbit axis
        var quat = new THREE.Quaternion().setFromUnitVectors( camera.up, new THREE.Vector3( 0, 1, 0 ) );
        var quatInverse = quat.clone().inverse();
        var lastQuaternion = new THREE.Quaternion();

        //2.为了计算移动点
        var lastPosition = new THREE.Vector3();

        return function(){
            //1.获取当前相机的位置的指针进行修改
            var position = this.camera.position;
            //2.计算出相机和相机照看的目标的偏移值向量；
            offset.copy(position).sub(this.target);
            //3.旋转偏移值向量到指定象限空间，用于计算
            offset.applyQuaternion(quat);

            //4.计算出当前在x-z平面的旋转度
            theta = Math.atan2(offset.x,offset.z);
            //5.得出在x-z平面要旋转到的角度
            theta += thetaDelta;
            theta = Math.max(this.minAzimuthAngle,Math.min(this.maxAzimuthAngle,theta));

            //6.计算出当前相对y轴的旋转度
            phi = Math.atan2(Math.sqrt(offset.x*offset.x + offset.z*offset.z),offset.y);
            //7.得出在y轴要旋转到的角度
            phi += phiDelta;
            phi = Math.max(this.minPolarAngle,Math.min(this.maxPolarAngle,phi));
            phi = Math.max(EPS,Math.min(Math.PI - EPS, phi));

            //8.计算相机准备旋转的半径，如何有缩放，需要对应的缩放值
            var radius = offset.length()*scale;

            //9.按限定条件，规范半径到限定范围  min<= radius <= max
            radius = Math.max(this.minDistance, Math.min(this.maxDistance,radius));

            //10.计算偏移向量旋转后的各个距离值
            offset.x = radius * Math.sin(theta)*Math.sin(phi);
            offset.z = radius * Math.cos(theta)*Math.sin(phi);
            offset.y = radius * Math.cos(phi);

            //11. 完成旋转计算，回归偏移值向量到原始象限
            offset.applyQuaternion(quatInverse);

            //12.移到目标到偏移的平面量
            this.target.add(panOffset);

            //13.修改相机的新向量位置
            position.copy(this.target).add(offset);

            //14.继续让相机看向目标
            this.camera.lookAt(this.target);


            //15.偏移完成将传入偏移值进行复位，等待新偏移值的传入
            thetaDelta = 0;
            phiDelta = 0;
            scale = 1;
            panOffset.set(0,0,0);


            //13.规定更新的频率
            // update condition is:
            // min(camera displacement, camera rotation in radians)^2 > EPS
            // using small-angle approximation cos(x/2) = 1 - x^2 / 8
            if ( zoomChanged ||
                 lastPosition.distanceToSquared( this.camera.position ) > EPS ||
                8 * ( 1 - lastQuaternion.dot( this.camera.quaternion ) ) > EPS ) {

                lastPosition.copy( this.camera.position );
                lastQuaternion.copy( this.camera.quaternion );
                zoomChanged = false;

                return true;

            }

            return false;

        }


    }();


    this.updatePanMove = function(){
        var offset = new THREE.Vector3();

        var lastPostion = new THREE.Vector3();
        return function(){
            //1.获取当前相机的位置的指针进行修改
            var position = this.camera.position;
            //2.计算出相机和相机照看的目标的偏移值向量；
            offset.copy(position).sub(this.target);

            //3.依据传入的偏移值移动目标到指定位置
            this.target.add(panOffset);

            //4.将相机移到目标，然后加上原始的偏移值向量，完成相机的移动
            position.copy(this.target).add(offset);

            //5.让相机看向新的目标位置
            this.camera.lookAt(this.target);

            //6.完成平移，重置平移向量
            panOffset.set(0,0,0);
            return true;

        }

    }();

    this.updateZoom = function(){
        return function(){

            if(zoomChanged){
                zoomChanged = false;
                return true;
            }
            return false;
        }


    }();


    this.rotateYAxisUpdate = function(){
        var offset = new THREE.Vector3();


        // 1.为了旋转 so camera.up is the orbit axis
        var quat = new THREE.Quaternion().setFromUnitVectors( camera.up, new THREE.Vector3( 0, 1, 0 ) );
        var quatInverse = quat.clone().inverse();
        var lastQuaternion = new THREE.Quaternion();

        //2.为了平移






        return function(){
            //1.获取当前相机的位置的指针进行修改
            var position = this.camera.position;
            //2.计算出相机和相机照看的目标的偏移值向量；
            offset.copy(position).sub(this.target);
            //3.旋转偏移值向量到指定象限空间，用于计算
            offset.applyQuaternion(quat);

            //4.计算出当前在x-z平面的旋转度
            theta = Math.atan2(offset.x,offset.z);
            //5.得出在x-z平面要旋转到的角度
            theta += thetaDelta;
            theta = Math.max(this.minAzimuthAngle,Math.min(this.maxAzimuthAngle,theta));

            //6.计算出当前相对y轴的旋转度
            phi = Math.atan2(Math.sqrt(offset.x*offset.x + offset.z*offset.z),offset.y);
            //7.得出在y轴要旋转到的角度
            phi += phiDelta;
            phi = Math.max(this.minPolarAngle,Math.min(this.maxPolarAngle,phi));
            phi = Math.max(EPS,Math.min(Math.PI - EPS, phi));

            //6.计算相机准备旋转的半径，如何有缩放，需要对应的缩放值
            var radius = offset.length()*scale;

            //7.按限定条件，规范半径到限定范围  min<= radius <= max
            radius = Math.max(this.minDistance, Math.min(this.maxDistance,radius));

            //8.计算偏移向量旋转后的各个距离值
            offset.x = radius * Math.sin(theta)*Math.sin(phi);
            offset.z = radius * Math.cos(theta)*Math.sin(phi);
            offset.y = radius * Math.cos(phi);

            //9. 完成计算，回归偏移值向量到原始象限
            offset.applyQuaternion(quatInverse);



            //






            //10.修改相机的新向量位置
            position.copy(this.target).add(offset);

            //11.继续让相机看向目标
            this.camera.lookAt(this.target);


            //12.偏移完成将传入偏移值进行复位，等待新偏移值的传入
            thetaDelta = 0;
            phiDelta = 0;


            //13.规定更新的频率
            // update condition is:
            // min(camera displacement, camera rotation in radians)^2 > EPS
            // using small-angle approximation cos(x/2) = 1 - x^2 / 8
//            if ( 8 * ( 1 - lastQuaternion.dot( this.camera.quaternion ) ) > EPS ) {
//                lastQuaternion.copy( this.camera.quaternion );
//                return true;
//            }

            return true;

        }


    }();


    this.rotateXAxisUpdate = function(){
        var offset = new THREE.Vector3();

    }();


};
// This set of controls performs orbiting, dollying (zooming), and panning. It maintains
// the "up" direction as +Y, unlike the TrackballControls. Touch on tablet and phones is
// supported.
//
//    Orbit - left mouse / touch: one finger move
//    Zoom - middle mouse, or mousewheel / touch: two finger spread or squish
//    Pan - right mouse, or arrow keys / touch: three finter swipe

THREE.OrbitControls = function(camera,_eventElement){

    this.update = function(){

        if( this.autoRotate && state === STATE.NONE){
            constraint.rotateLeft(2 * Math.PI / 60 / 60* this.autoRotateSpeed);
        }

        if(constraint.update() === true){
            this.dispatchEvent(changeEvent);
        }
    };
    var constraint = new OrbitConstraint(camera);
    this.domElement =  _eventElement;

    Object.defineProperty(this,'constraint',{
                              get:function(){
                                  return constraint;
                              }
                          });
    this.getPolarAngle = function(){
        return constraint.getAzimuthalAngle();
    };

    // Set to false to disable this control;
    this.enabled = true;

    // center is old, deprecated; use "target" instead
    this.center = this.target;

    this.enableZoom = true;
    this.zoomSpeed = 1.0;


    this.enableRotate = true;
    this.rotateSpeed = 1.0;

    this.enablePan = true;
    this.keyPanSpeed = 7.0; //pixels moved per arrow key pus

    this.autoRotate = false;
    this.autoRotateSpeed =2.0; //// 30 seconds per round when fps is 60

    // Set to false to disable use of the keys
    this.enableKeys = true;
    // The four arrow keys
    this.keys = {LEFT:37, UP:38, RIGHT:39, BOTTOM:40};
    // Mouse buttons
    this.mouseButtons = {ORBIT: THREE.MOUSE.LEFT,ZOOM: THREE.MOUSE.MIDDLE, PAN: THREE.MOUSE.RIGHT};


    //for reset

    this.target0 = this.target.clone();
    this.position0 = this.camera.position.clone();
    this.zoom0 = this.camera.zoom;
    //internals

    var me = this;

    var rotateStart = new THREE.Vector2();
    var rotateEnd = new THREE.Vector2();
    var rotateDelta = new THREE.Vector2();

    var panStart = new THREE.Vector2();
    var panEnd = new THREE.Vector2();
    var panDelta = new THREE.Vector2();

    var dollyStart = new THREE.Vector2();
    var dollyEnd = new THREE.Vector2();
    var dollyDelta = new THREE.Vector2();

    var STATE ={NONE: -1, ROTATE:0, DOLLY: 1, PAN: 2, TOUCH_ROTATE: 3, TOUCH_DOLLY: 4, TOUCH_PAN: 5};
    var state = STATE.NONE;
    //events

    var changeEvent = {type:'change'};
    var startEvent = {type:'start'};
    var endEvent = {type:'end'};



    this.reset = function(){
        state = STATE.NONE;

        this.target.copy(this.target0);
        this.camera.position.copy(this.position0);
        this.camera.zoom = this.zoom0;

        this.camera.updateProjectionMatrix();
        this.dispatchEvent(changeEvent);

        this.update();
    }





    this.dispose = function(){

//        this.domElement.removeEventListener( 'contextmenu', contextmenu, false );
        this.domElement.removeEventListener( 'mousedown', onDocumentMouseDown, false );
        this.domElement.removeEventListener( 'mousewheel', onDocumentMouseWheel, false );

        this.domElement.removeEventListener( 'mousemove', onDocumentMouseMove, false );
        this.domElement.removeEventListener( 'mouseup', onDocumentMouseUp, false );

        this.domElement.removeEventListener( 'keydown', onDocumentKeyDown, false );
        this.update();
    }



    function onDocumentMouseWheel(event){
        if(me.enabled === false || me.enableZoom === false || state !== STATE.NONE) return;

        var t_delta =0;
        if(event.wheelDelta !== undefined){
            t_delta = event.wheelDelta;
        }

        if(t_delta > 0){
            constraint.dollyOut(me.zoomSpeed);
        } else if(t_delta < 0){
            constraint.dollyIn(me.zoomSpeed);
        }

        me.update();
        me.dispatchEvent(startEvent);
        me.dispatchEvent(endEvent);
    }
    function onDocumentKeyDown(event){


        if(me.enabled ===  false || me.enableKeys === me || me.enablePan === false) return;
        console.log("1111111111111111111111111111111")

        switch(event.keyCode){
        case me.keys.UP:
            __pan(0,this.keyPanSpeed);

            me.update();
            break;
        case me.keys.BOTTOM:
            __pan(0,- this.keyPanSpeed);
            me.update();
            break;
        case me.keys.LEFT:
            __pan(this.keyPanSpeed,0);
            me.update();
            break;
        case me.keys.RIGHT:
            __pan( - this.keyPanSpeed,0);
            me.update();
            break;
        }

    }
    function onDocumentMouseDown(event){
        if(me.enabled === false) return;
//            event.preventDefault();

        if(event.button === me.mouseButtons.ORBIT){
            if(me.enableRotate === false) return;
            state = STATE.ROTATE;
            rotateStart.set(event.clientX,event.clientY);
        } else if(event.button === me.mouseButtons.ZOOM){

            if ( me.enableZoom === false ) return;

            state = STATE.DOLLY;

            dollyStart.set( event.clientX, event.clientY );

        } else if ( event.button === me.mouseButtons.PAN ) {

            if ( me.enablePan === false ) return;

            state = STATE.PAN;

            panStart.set( event.clientX, event.clientY );

        }

        if(state !== STATE.NONE){
            _eventElement.addEventListener( 'mousemove', onDocumentMouseMove, false );
            _eventElement.addEventListener( 'mouseup', onDocumentMouseUp, false );
            me.dispatchEvent(startEvent);

        }
    }
    function onDocumentMouseMove(event){
        if(me.enabled === false) return;

        if(state === STATE.ROTATE){
            __cameraRotate(event);
        }else if( state === STATE.DOLLY){
            __cameraZoom(event);

        }else if( state === STATE.PAN){
            __cameraPan(event);
        }

        if(state !== STATE.NONE) me.update();

    }
    function onDocumentMouseUp(event){
        if(me.enabled === false) return;
        me.domElement.removeEventListener('mousemove',onDocumentMouseMove,false);
        me.domElement.removeEventListener('mouseup',onDocumentMouseUp,false);
        me.dispatchEvent( endEvent );
        state = STATE.NONE;
    }



    function __cameraRotate(event){
        //1.准备旋转
        if(me.enableRotate === false) return;
        rotateEnd.set(event.clientX,event.clientY);
        rotateDelta.subVectors(rotateEnd,rotateStart);

        //2.开始旋转 rotating across whole screen goes 360 degrees around
        constraint.rotateLeft(2*Math.PI*rotateDelta.x/ me.domElement.width*me.rotateSpeed);
        constraint.rotateUp(2*Math.PI*rotateDelta.y/me.domElement.height*me.rotateSpeed);

        //3.完成旋转
        rotateStart.copy(rotateEnd);
    }

    function __cameraZoom(event){
        //1.准备缩放
        if(me.enableRotate === false) return;
        rotateEnd.set(event.clientX,event.clientY);
        rotateDelta.subVectors(rotateEnd,rotateStart);

        //2.开始旋转 rotating across whole screen goes 360 degrees around
        constraint.rotateLeft(2*Math.PI*rotateDelta.x/ me.domElement.width*me.rotateSpeed);
        constraint.rotateUp(2*Math.PI*rotateDelta.y/me.domElement.height*me.rotateSpeed);

        //3.完成旋转
        rotateStart.copy(rotateEnd);

    }

    function __cameraPan(event){

        //1.准备平移
        if(me.enablePan === false) return;
        panEnd.set(event.clientX,event.clientY);
        panDelta.subVectors(panEnd,panStart);

        //2.开始平移
        __pan(panDelta.x, panDelta.y);
        //3.完成平移
        panStart.copy(panEnd);

    }

    function __pan(deltaX,deltaY){
        constraint.pan(deltaX,deltaY,_eventElement.width,_eventElement.height);
    }




    // init
//    this.domElement.addEventListener('contextmenu',onDocumentContextMenu,false);
    this.domElement.addEventListener('mousedown',onDocumentMouseDown,false);
    this.domElement.addEventListener('mousewheel',onDocumentMouseWheel,false);
//    this.domElement.addEventListener('keydown',onDocumentKeyDown,false);
    this.update();



}

THREE.OrbitControls.prototype = Object.create(THREE.EventDispatcher.prototype);
THREE.OrbitControls.prototype.constructor = THREE.OrbitControls;


Object.defineProperties(THREE.OrbitControls.prototype,{
                            camera:{
                                get: function(){
                                    return this.constraint.camera;
                                }
                            },
                            target:{
                                get:function(){
                                    return this.constraint.target;
                                },
                                set:function(value){
                                    console.warn( 'THREE.OrbitControls: target is now immutable. Use target.set() instead.' );
                                    this.constraint.target.copy( value );
                                }
                            },
                            minDistance:{
                                get:function(){
                                    return this.constraint.minDistance;
                                },
                                set:function(value){
                                    this.constraint.minDistance = value;
                                }
                            },
                            maxDistance:{
                                get: function(){
                                    return this.constraint.maxDistance;
                                },
                                set:function(value){
                                    this.constraint.maxDistance = value;
                                }
                            },
                            minZoom:{
                                get: function(){
                                    return this.constraint.minZoom;
                                },
                                set:function(value){
                                    this.constraint.minZoom = value;
                                }
                            },
                            maxZoom:{
                                get:function(){
                                    return this.constraint.maxZoom;
                                },
                                set:function(value){
                                    this.constraint.maxZoom = value;
                                }
                            },
                            minPolarAngle : {

                                get: function () {

                                    return this.constraint.minPolarAngle;

                                },

                                set: function ( value ) {

                                    this.constraint.minPolarAngle = value;

                                }

                            },

                            maxPolarAngle : {

                                get: function () {

                                    return this.constraint.maxPolarAngle;

                                },

                                set: function ( value ) {

                                    this.constraint.maxPolarAngle = value;

                                }

                            },

                            minAzimuthAngle : {

                                get: function () {

                                    return this.constraint.minAzimuthAngle;

                                },

                                set: function ( value ) {

                                    this.constraint.minAzimuthAngle = value;

                                }

                            },

                            maxAzimuthAngle : {

                                get: function () {

                                    return this.constraint.maxAzimuthAngle;

                                },

                                set: function ( value ) {

                                    this.constraint.maxAzimuthAngle = value;

                                }

                            },

                            enableDamping : {

                                get: function () {

                                    return this.constraint.enableDamping;

                                },

                                set: function ( value ) {

                                    this.constraint.enableDamping = value;

                                }

                            },

                            dampingFactor : {

                                get: function () {

                                    return this.constraint.dampingFactor;

                                },

                                set: function ( value ) {

                                    this.constraint.dampingFactor = value;

                                }

                            }

})




