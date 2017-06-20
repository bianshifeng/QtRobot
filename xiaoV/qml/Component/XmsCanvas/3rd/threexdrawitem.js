//Qt.include("./three.js")



THREE.AlgDrawItem = function(scene,camera,backPlaneMesh,_eventElement){
    this.uuid = THREE.Math.generateUUID();
    this.type = "";
    console.log(this.uuid)
    Object.defineProperty( this, 'id', { value: this.uuid } );
    Object.defineProperty(this,'type',{value:this.type});

    this.keys = {LEFT:37, UP:38, RIGHT:39, BOTTOM:40};
    // Mouse buttons
    this.mouseButtons = {ORBIT: THREE.MOUSE.LEFT,ZOOM: THREE.MOUSE.MIDDLE, PAN: THREE.MOUSE.RIGHT};



    var _mouse = new THREE.Vector3();
    var _raycaster = new THREE.Raycaster();
    var _isShiftDown = false;
    var _selected, _hovered;

    var objectMesh;
    var pointMeshList = [];
    var lineMeshList = [];
    var shapeMesh;
    var pointsLength = 0;
    var pointPostions=[];



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

    this.setPointsData = function(data){
        var t_data =data
        var t_point = new THREE.Vector3()
        pointPostions=[];
        console.log("111111111111111111111")
        t_data.forEach(function(point){
            t_point= new THREE.Vector3(point.x,point.y,10)
            pointPostions.push(t_point)
        })

        pointsLength = pointPostions.length;


    }

    this.getPointsData = function(){

        var t_point;
        var t_data =[]
        pointPostions.forEach(function(point){
            t_point={x:point.x,y:point.y}
            t_data.push(t_point)

        })

        return t_data;

    }


    this.setActiveObjectManager = function(activeObjectManager){
        for(var i = 0; i < pointMeshList.length; i++){
            algObjects.push(pointMeshList[i]);
        }

    }


    this.draw = function(algObjects){
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

        for(i = 0; i< pointsLength; i++){
            t_geomerty.vertices.push(pointPostions[i]);
        }

        // 闭合线条
        t_geomerty.vertices.push(pointPostions[0]);
        var t_material =new THREE.LineBasicMaterial(
                    {color: 0xff0000,opacity: 0.35,linewidth: 6});

        state = STATE.MODIFY;
        objectMesh = new THREE.Line(t_geomerty,t_material);
        scene.add(objectMesh);



        for(i = 0; i < pointMeshList.length; i++){
            algObjects.push(pointMeshList[i]);
        }

    }
    this.update = function(){
        modifyGeometryByChange()
    }
    this.remove = function(algObjects){

        //1.从点控制数组中移除点
        for(var i = 0; i < pointMeshList.length; i++){
            algObjects.forEach(function(item){
                if(item.id === scope.uuid) algObjects.pop(item);
            })
        }

        //2.从舞台移除线
        if(objectMesh) scene.remove(objectMesh);

        //3.从舞台移除点
        pointMeshList.forEach(function(item){
            scene.remove(item);
        })
    }


    function modifyGeometryByChange(){
        //1.方法一： 直接更新对应移动点的结构体坐标
        var p;
        var t_leng = pointMeshList.length;
        pointPostions=[]


        for(var i = 0; i <t_leng ; i++){
            p = objectMesh.geometry.vertices[ i ];
            p.copy(pointMeshList[i].position);
            pointPostions.push(pointMeshList[i].position);
        }
        p = objectMesh.geometry.vertices[t_leng];
        p.copy(pointMeshList[0].position);


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
        scope.update();
        return true;
    }

    function drawPointObject(position){
        var t_geometry = new THREE.SphereGeometry(10,10,10);
        var t_material = new THREE.MeshLambertMaterial({color:0xffff00});
//        var t_material = new THREE.MeshBasicMaterial( {color: 0xffff00} );
        var t_object = new THREE.Mesh(t_geometry,t_material);
        t_geometry.name = scope.uuid;
        console.log(t_geometry.name)

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

}
