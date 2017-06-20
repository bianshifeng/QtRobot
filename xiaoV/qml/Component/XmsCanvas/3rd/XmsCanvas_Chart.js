Qt.include("./three.js")

//基础和相机类型
var camera, scene, renderer;
var sceneAnimationClip;
var mixer;




var isPlane = false;
var GL_Backgroud_Color ="#123456"


//qt画布对象和事件触发者
var canvas, domElement;

//碰撞检测器
var raycaster, mouse;


function initializeGL(_canvas,_domElement) {
    canvas = _canvas;
    domElement = _domElement;

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

    __createTest();


}



function resizeGL(_canvas) {
    if(!canvas)canvas = _canvas;
    camera.aspect = canvas.width / canvas.height;
    camera.updateProjectionMatrix();
    renderer.setPixelRatio(canvas.devicePixelRatio);
    renderer.setSize(canvas.width, canvas.height);
}

function paintGL(_canvas) {
    if(!canvas)canvas = _canvas;
    renderer.render( scene, camera );

}

function __createTest(){
//    var material = new THREE.MeshBasicMaterial({ color: 0x80c342,
//                                                   ambient: 0x000000,
//                                                   shading: THREE.SmoothShading });
//    var cubeGeometry = new THREE.BoxGeometry(200, 200, 200);
//    var cube = new THREE.Mesh(cubeGeometry, material);
//    cube.rotation.set(0.785, 0.785, 0.0);
//    scene.add(cube);


    var loader = new THREE.ObjectLoader();

    loader.load( "../models/scene-animation.json", function ( loadedScene ) {
        console.log(JSON.stringify(loadedScene))
        sceneAnimationClip = loadedScene.animations[0];
        mixer = new THREE.AnimationMixer( scene );
        mixer.addAction( new THREE.AnimationAction( sceneAnimationClip ) );
    } );



//    var geometry = new THREE.SphereGeometry( 100, 20, 20 );

//    var texture = THREE.ImageUtils.loadTexture("../textures/land_ocean_ice_cloud_2048.jpg")

//    var material = new THREE.MeshLambertMaterial( { map:texture , overdraw: 0.5 } );
//    var mesh = new THREE.Mesh( geometry, material );
//    scene.add( mesh );


//    var loader = new THREE.TextureLoader();

//    loader.load( '../textures/land_ocean_ice_cloud_2048.jpg', function ( texture ) {
//        var geometry = new THREE.SphereGeometry( 300, 20, 20 );

//        var material = new THREE.MeshLambertMaterial( { map: texture, overdraw: 0.5 } );
//        var mesh = new THREE.Mesh( geometry, material );
//        scene.add( mesh );
//    } );



}


















function __createScene(){
    //1.create scene
    console.debug("1.__createScene")
    scene = new THREE.Scene();
//    scene.fog = new THREE.Fog(0x050505,2000,3500);
    scene.fog = new THREE.Fog( 0xffffff, 2000, 10000 );
}
function __createCamera(){
    //2.create camera

    if(isPlane){
        console.debug("2.__create OrthographicCamera")
        camera = new THREE.OrthographicCamera(canvas.width / -2, canvas.width / 2, canvas.height / 2, canvas.height / -2, -2000, 2000);
        camera.position.x = 0;
        camera.position.y = 0;
        camera.position.z = 1000;
        camera.zoom = Math.min(canvas.width/canvas.imageWidth,canvas.height/canvas.imageHeight)
        camera.lookAt(scene.position);
    }else{

        console.debug("2.__create PerspectiveCamera")
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
    console.debug("3.__create PerspectiveCamera")
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
    console.debug("4.__create renderer")
    renderer = new THREE.Canvas3DRenderer(
                {
                    canvas: canvas,
                    antialias: true,
                    alpha:true,
                    devicePixelRatio:
                    canvas.devicePixelRatio
                });
    // create a render and set the size
    renderer.setClearColor(new THREE.Color(0xeeeeee));
    renderer.setSize(canvas.width, canvas.height);
}
function __createBackgroud(){
    console.debug("5.__create Backgroud")

    //1.create backgroud
//    var planeGeometry = new THREE.PlaneGeometry(16000,16000);
////    planeGeometry.rotateX( - Math.PI /2);
//    var planeMaterial = new THREE.MeshBasicMaterial({color:0xeeeeee});
//    var plane = new THREE.Mesh(planeGeometry,planeMaterial);
//    plane.position.y = -200;
//    plane.rotation.x = - Math.PI /2;
//    plane.receiveShadow = true;
//    scene.add(plane);

    // create x-z plane
    var geometry = new THREE.PlaneBufferGeometry( 2000, 2000 );
    var material = new THREE.MeshBasicMaterial({color:GL_Backgroud_Color}); //new THREE.MeshPhongMaterial( { emissive: 0xeeeeee } );

    var ground = new THREE.Mesh( geometry, material );
    ground.position.set( 0, -200, 0 );
    ground.rotation.x = -Math.PI/2;
    ground.receiveShadow = true;
    scene.add( ground )



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

