THREE.AlgDrawItemNew = function(scene,camera,backPlaneMesh,_eventElement,_data){
    this.uuid = THREE.Math.generateUUID();
    console.log("dfsdfsfsfd"+this.uuid);
    Object.defineProperty( this, 'id', { value: this.uuid } );

    this.keys = {LEFT:37, UP:38, RIGHT:39, BOTTOM:40};
    // Mouse buttons
    this.mouseButtons = {ORBIT: THREE.MOUSE.LEFT,ZOOM: THREE.MOUSE.MIDDLE, PAN: THREE.MOUSE.RIGHT};



    var point1 = new THREE.Vector3(0,0,1);
    var point2 = new THREE.Vector3(0,100,1);
    var point3 = new THREE.Vector3(100,100,1);
    var point4 = new THREE.Vector3(100,0,1);

    var _mouse = new THREE.Vector3();
    var _raycaster = new THREE.Raycaster();
    var _isShiftDown = false;
    var _selected, _hovered;

    var objectMesh;
    var pointMeshList = [];
    var lineMeshList = [];
    var shapeMesh;
    var pointsLength = 4;
    var pointPostions=[point1,point2,point3,point4];



    //custom event handing
    var scope = this;
    var _eventListeners ={};
    var STATE = {DELETE:-1,NONE:0, DRAW: 1, MODIFY:2};
    var state = STATE.NONE;



    this.on = function(event,handler){
        if(!_eventListeners[event]){
            _eventListeners[event] = [];
        }

        _eventListeners[event].push(handler);
        return scope;
    };

    this.off = function(event,handler){
        var t_event_hanlders = _eventListeners[event];
        if(!t_event_hanlders) return scope;

        if(t_event_hanlders.indexOf(handler) > -1){
            t_event_hanlders.splice(handler,1);
        }
        return scope;
    }

    var notify = function(event,data,member){
        var t_event_hanlders = _eventListeners[event];
        if(!t_event_hanlders) return;

        if(!member){
            for(var i = 0; i < t_event_hanlders.length; i++){
                t_event_hanlders[i](data);
            }
        }

    };


    this.setObjectManager = function(algObjects){
        for(var i = 0; i < pointMeshList.length; i++){
            algObjects.push(pointMeshList[i]);
        }
    }

    this.setActiveObjectManager = function(activeObjectManager){
        for(var i = 0; i < pointMeshList.length; i++){
            algObjects.push(pointMeshList[i]);
        }

    }

    this.draw = function(){

        notify('create',this);
        this.activate();
        //1.create point object
        var i;
        var t_length = pointPostions.length;

        //2.create line object

        //3.create shape object

    }

    this.draw_old = function(){

        //1.创建已有点
        var i;
        var t_length = pointPostions.length;
        for(i = 0; i < t_length; i++){
            drawPointObject(pointPostions[i])
        }
        pointPostions =[];
        for(i = 0; i<t_length; i++){
            pointPostions.push(pointMeshList[i].position);

        }


        //2.创建一个划线的空结构体，
        var t_geomerty = new THREE.Geometry();

        for(i = 0; i< 80; i++){
            t_geomerty.vertices.push(new THREE.Vector3());
        }

        var t_material =new THREE.LineBasicMaterial(
                    {color: 0xff0000,opacity: 0.35,linewidth: 6});

        state = STATE.MODIFY;

        objectMesh = new THREE.Line(t_geomerty,t_material);
        //

        scene.add(objectMesh);

    }

    this.update = function(){
//        modifyGeometryByChange()
    }

    this.remove = function(){
        if(objectMesh) scene.remove(objectMesh);

        pointMeshList.forEach(function(item){
            scene.remove(item);
        })

    }

    function modifyGeometryByChange(){
        //1.方法一： 直接更新对应移动点的结构体坐标
        var p;
        var t_leng = pointMeshList.length;
        for(var i = 0; i <t_leng ; i++){
            p = objectMesh.geometry.vertices[ i ];
            p.copy(pointMeshList[i].position);
        }
//        p = objectMesh.geometry.vertices[t_leng];
//        p.copy(pointMeshList[0].position);


        objectMesh.geometry.verticesNeedUpdate = true;
    }

    function drawGeometryByNew() {

        //2. 方法二： 使用新移动点重新构建一个结构体，直接替换

        var t_newGeometry = new THREE.Geometry();

        var p;
        for(var i = 0; i < pointMeshList.length; i++){
            t_newGeometry.vertices.push(pointMeshList[i].position.clone());
        }

        objectMesh.geometry.dispose();
        objectMesh.geometry = t_newGeometry;

//        mesh.children[0].geometry = new THREE.WireframeGeometry( geometry );
//        mesh.children[1].geometry = geometry;

        //these do not update nicely together if shared
    }






    function addPoint(new_point){
        pointPostions.push(drawPointObject(new_point).position);

        var t_length = pointPostions.length;
        if(t_length >= 2){
            drawLineObject(pointPostions[t_length-2],pointPostions[t_length-1])
        }
        scope.update();
        return true;
    }


    function drawLineObject(position_s,position_e){

        var t_geomerty = new THREE.Geometry();
        t_geomerty.vertices.push(position_s);
        t_geomerty.vertices.push(position_e);
        t_geomerty.name = this.uuid;
        var t_material =new THREE.LineBasicMaterial(
                    {color: 0xff0000,opacity: 0.35,linewidth: 6});
        var t_object = new THREE.Line(t_geomerty,t_material);

        scene.add(t_object);
        lineMeshList.push(t_object)
        return t_object;
    }

    function drawPointObject(position){
        var t_geometry = new THREE.BoxGeometry(20,20,20);
        var t_material = new THREE.MeshLambertMaterial({color:0xffffff});
        var t_object = new THREE.Mesh(t_geometry,t_material);
        t_geometry.name = this.uuid;

        console.log("1212121");
        console.log(position.x);
        console.log(position.y);
        console.log(position.z);

        if(position){
            t_object.position.copy(position);
        }else{
            t_object.position.x = 300;
            t_object.position.y = 400;
            t_object.position.z = 500;
        }

        scene.add(t_object);
        pointMeshList.push(t_object)
        return t_object;
    }



    this.activate = function(){
        _eventElement.addEventListener('mousemove',onDocumentMouseMove,false);
        _eventElement.addEventListener('mousedown',onDocumentDrawMouseDown,false);
        _eventElement.addEventListener('mouseup',onDocumentDrawMouseUp,false);

        console.log("the draw item active");
    }

    this.deactivate = function(){
        _eventElement.removeEventListener('mousemove',onDocumentMouseMove,false);
        _eventElement.removeEventListener('mousedown',onDocumentMouseDown,false);
        _eventElement.removeEventListener('mouseup',onDocumentMouseUp,false);

    }

    function onDocumentDrawMouseDown(event){
        _mouse.x =(event.clientX/_eventElement.width) * 2 -1;
        _mouse.y = -(event.clientY/_eventElement.height) *2 +1;
    }

    function onDocumentDrawMouseUp(event){
        _mouse.x =(event.clientX/_eventElement.width) * 2 -1;
        _mouse.y = -(event.clientY/_eventElement.height) *2 +1;

        _raycaster.setFromCamera( _mouse, camera );


        var intersects;
        if(_isShiftDown){
            intersects = _raycaster.intersectObjects( pointMeshList );
        }else{
            intersects = _raycaster.intersectObjects( [backPlaneMesh] );
            if ( intersects.length > 0 ) {
                var intersect = intersects[ 0 ];
                addPoint(intersect.point);
                notify('addPoint',_mouse);
            }
        }
    }


    function onDocumentMouseMove(event){};
    function onDocumentMouseDown(event){};
    function onDocumentMouseUp(event){};

    this.dispose = function(){
        scope.deactivate();
    }

}


