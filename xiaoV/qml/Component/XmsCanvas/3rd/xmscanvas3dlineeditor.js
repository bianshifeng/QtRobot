Qt.include("./three.js")
Qt.include("./threexorbitcontrols.js")
Qt.include("./threexdragcontrols.js")
Qt.include('./threexdrawitem.js')

//基础和相机类型
var camera, scene, renderer;
var isPlane = true;

//qt画布对象和事件触发者
var canvas, domElement;

//碰撞检测器
var raycaster, mouse;

//全局鼠标控制器
var dragcontrols ;
var orbitcontrols;

var algDragObjects =[];
var algItems =[];
var activeAlgObject;
var activeAlgItem;
var activeAlgItemUuid;

var imagePlaneMesh;
var imagePlaneMateral;



function initializeGL(qmlCanvas,eventElement){


    canvas = qmlCanvas;
    domElement = eventElement
    mouse = new THREE.Vector2();

    function __createScene(){
        //1.create scene
        scene = new THREE.Scene();
        scene.fog = new THREE.Fog(0x050505,2000,3500);
    }
    function __createCamera(){
        //2.create camera

        if(isPlane){
            camera = new THREE.OrthographicCamera(canvas.width / -2, canvas.width / 2, canvas.height / 2, canvas.height / -2, -2000, 2000);
            camera.position.x = 0;
            camera.position.y = 0;
            camera.position.z = 1000;
            camera.zoom = Math.min(canvas.width/canvas.imageWidth,canvas.height/canvas.imageHeight)
            camera.lookAt(scene.position);
        }else{

            camera = new THREE.PerspectiveCamera( 45, canvas.width / canvas.height, 1, 10000 );
            camera.position.x = 0;
            camera.position.y = 0;
            camera.position.z = 1000;
            camera.lookAt(scene.position);
        }
        scene.add(camera);


        //Three.JS 从世界坐标系转换到屏幕坐标系
    //    var width = canvas.width, height = canvas.height;
    //    var widthHalf = width / 2, heightHalf = height / 2;
    //    var vector = new THREE.Vector3();
    //    var projector = new THREE.Projector();
    //    projector.projectVector( vector.getPositionFromMatrix( object.matrixWorld ), camera );
    //    vector.x = ( vector.x * widthHalf ) + widthHalf;
    //    vector.y = - ( vector.y * heightHalf ) + heightHalf;

    }
    function __createLights(){

        //1.create light
        var t_ambientLight = new THREE.AmbientLight(0xf0f0f0)
        scene.add(t_ambientLight);

        //2.create spotlight
        var light = new THREE.SpotLight(0xffffff,1.5);
        light.position.set(0,1500,200);
        light.castShadow =true;
        light.shadowCameraNear = 200;
        light.shadowCameraFar = camera.far;
        light.shadowCameraFov = 70;
        light.shadowBias = -0.000222;
        light.shadowDarkness = 0.25;
        light.shadowMapWidth = 1024;
        light.shadowMapHeight = 1024;
        scene.add(light);

        var spotlight = light;

    }
    function __createRenderer(){
        //4.create render
        renderer = new THREE.Canvas3DRenderer(
                    { canvas: canvas, antialias: true, devicePixelRatio: canvas.devicePixelRatio });
        // create a render and set the size
//        renderer.setClearColor(new THREE.Color(0xeeeeee));
        renderer.setSize(canvas.width, canvas.height);
    }
    function __createBackgroud(){

        //1.create backgroud
    //    var planeGeometry = new THREE.PlaneGeometry(2000,2000);
    ////    planeGeometry.rotateX( - Math.PI /2);
    //    var planeMaterial = new THREE.MeshBasicMaterial({color:0xeeeeee});
    //    var plane = new THREE.Mesh(planeGeometry,planeMaterial);
    //    plane.position.y = -200;
    //    plane.rotation.x = - Math.PI /2;
    //    plane.receiveShadow = true;
    //    scene.add(plane);


    //    var planeGeometry = new THREE.PlaneGeometry( 2000, 2000 );
    //    var planeMaterial = new THREE.MeshBasicMaterial( { color: 0xeeeeee } );
    //    var plane = new THREE.Mesh( planeGeometry, planeMaterial );
    //    plane.position.y = -200;
    //    plane.rotation.x =- Math.PI / 2
    //    plane.receiveShadow = true;
    //    scene.add( plane );


        //3.create grid helper
        var helper = new THREE.GridHelper( 1000, 100 );
        helper.position.y = - 199;
        helper.material.opacity = 0.25;
        helper.material.transparent = true;
        scene.add( helper );



        //3.create axis helper
        var axis = new THREE.AxisHelper();
        axis.position.set( -500, -500, -500 );
        scene.add( axis );



        //3.create back object to anchor the mouse



    //    //3.create helper
    //    var helper = new THREE.GridHelper(1000,100);
    //    helper.position.y = -199;
    //    helper.material.opacity = 0.25;
    //    helper.material.transparent = true;
    //    scene.add(helper);

    }
    function __createSnapImage(mapTexture) {

        var texture = THREE.ImageUtils.loadTexture(mapTexture)

        var t_geometry = new THREE.PlaneGeometry(canvas.imageWidth,canvas.imageHeight);
        imagePlaneMateral = new THREE.MeshBasicMaterial({
                                                       map: texture,
    //                                                   side: THREE.BackSide
                                                   })
        var geometry = new THREE.BufferGeometry().fromGeometry(t_geometry);
        imagePlaneMesh = new THREE.Mesh(geometry, imagePlaneMateral);
        scene.add(imagePlaneMesh);
    }

    function __createMouseControl(){

        //1.场景鼠标事件控制器，使用了three原型的eventlistener 设计模式
        orbitcontrols = new THREE.OrbitControls(camera,eventElement );
        orbitcontrols.damping = 0.2;
        orbitcontrols.addEventListener( 'change', paintGL );


        //2.3d对象拖拽控制器，使用了发布者模式设计的事件监听
        dragcontrols = new THREE.DragControls(camera,algDragObjects,eventElement);
        dragcontrols.on('dragstart',function(e){
            orbitcontrols.enabled = false;

            console.log("selected one item")
            console.log(e.object.geometry.name);
            activeAlgItemUuid = e.object.geometry.name
        });

        dragcontrols.on('draging',function(e){
            algItems.forEach(function(item){
                item.update();
            })
        });

        dragcontrols.on('dragend',function(e){
            orbitcontrols.enabled = true;
        });



    }

    //1.create scene
    __createScene();
    //2.create camera
    __createCamera();
    //3.create light
    __createLights()
    //4.create renderer
    __createRenderer()
    //5.draw backgroud
    __createBackgroud()
    //6.draw controler
    __createMouseControl()


    //7.draw snap image
    __createSnapImage(qmlCanvas.snapImage);

}
function resizeGL(canvas){
    if(camera === undefined) return;
//

    if(camera instanceof THREE.PerspectiveCamera){
        camera.aspect = canvas.width/canvas.height;
    }
    if(camera instanceof THREE.OrthographicCamera){
        //保证直接显示图片
        camera.left = canvas.width / -2;
        camera.right = canvas.width / 2;
        camera.top = canvas.height /2;
        camera.bottom = canvas.height/-2
        camera.zoom = Math.min(canvas.width/canvas.imageWidth,canvas.height/canvas.imageHeight)
        //考虑到还原器会记录尺寸变化后的缩放数据，需要在这里重新设置初始值

    }


    camera.updateProjectionMatrix();
    renderer.setPixelRatio(canvas.devicePixelRatio);
    renderer.setSize(canvas.width, canvas.height);
}
function paintGL(){
//    orbitcontrols.update();
    renderer.render(scene,camera);

}



function addItem(itemType,pointList){
    var t_dragItem =new THREE.AlgDrawItem(scene,camera,imagePlaneMesh,domElement);
    t_dragItem.type = itemType
    t_dragItem.setPointsData(pointList)
    t_dragItem.draw(algDragObjects);
    algItems.push(t_dragItem);
    activeAlgItemUuid = t_dragItem.id

}
function getDrawItemsData(){

    var algItemsData=[]
    algItems.forEach(function(algItem){
        var t_item={type:algItem.type,data:algItem.getPointsData()}

        algItemsData.push(t_item)

    })


    return algItemsData ;

}

function clearItems(){
    var t_length =algItems.length;
    for(var i = 0; i< t_length; i++){
        var item = algItems.pop();
        item.remove(algDragObjects);
    }

}

function deleteItem(){
    var t_type =""
    var uuid = activeAlgItemUuid
    var t_length = algItems.length;
    for(var i= 0; i< t_length; i++){
        var item=algItems[i];
        if(item.id === uuid){
            t_type = item.type
            item.remove(algDragObjects)
            algItems.splice(i,1)
            activeAlgItemUuid=""
        }
    }

    return t_type;
}


function changeImageGL(imgUrl){
    var texture = THREE.ImageUtils.loadTexture(imgUrl);
    imagePlaneMateral.map = texture;
    paintGL();
}

function resetGL(){
    orbitcontrols.reset();
    paintGL();
}




