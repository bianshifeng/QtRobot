import QtQuick 2.2
import QtQuick.Controls 1.4

XmsCanvas3DLineEditor {
    id: id_root

    property url imgUrl:""
    property int imgWidth:1230
    property int imgHeight:600

    property int maxTripwireNum:1
    property int maxPerimeterNum:1

    property int currentTripwireNum:0
    property int currentPerimeterNum:0

    property bool isTripwireCanAdded: currentTripwireNum < maxTripwireNum ? true:false
    property bool isPerimeterCanAdded: currentPerimeterNum < maxPerimeterNum ? true:false

    property string typePerimeter: "algParamPerimeter"
    property string typeTripwire:"algParamTripwire"
    property bool isShowTestBar:false



    Column{
        spacing: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -10
        visible: isShowTestBar
        Row{
            spacing: 8
            Button{
                text:"reset"
                onClicked: id_root.__reset()
            }

            Button{
                text:"changesnapimage"
                property string xxx
                onClicked: id_root.__changeImage(id_root.imgUrl,id_root.imgWidth,id_root.imgHeight)
            }

            Button{
                text:"add one Perimeter "+ currentPerimeterNum
                enabled: id_root.isPerimeterCanAdded
                property string xxx
                onClicked:  id_root.addPerimeterItem()

            }

            Button{
                text:"add one Tripwire " + currentTripwireNum
                enabled: id_root.isTripwireCanAdded
                property string xxx
                onClicked: id_root.addTripwireItem()
            }


            Button{
                text:"delete one"
                property string xxx
                onClicked: id_root.deleteItem()
            }

            Button{
                text:"clear"
                property string xxx
                onClicked: id_root.clearItem()
            }


            Button{
                text:"getContent"
                onClicked: {
                    var xxx =id_root.getTripwiresParam()
                    console.log(JSON.stringify(xxx))
                }
            }


            Button{
                text:"LoadTripwiresData"
                onClicked: {

                    var strPEAParam = "{
                        \"algParam\":
                        {
                            \"algParamPerimeter\":[
                                [
                                    {\"algPointX\":10,\"algPointY\":11},
                                    {\"algPointX\":20,\"algPointY\":21},
                                    {\"algPointX\":30,\"algPointY\":31},
                                    {\"algPointX\":40,\"algPointY\":41}]],
                            \"algParamTripwire\":[
                                {
                                    \"algTripwireEnd\":{\"algPointX\":200,\"algPointY\":210},
                                    \"algTripwireStart\":{\"algPointX\":100,\"algPointY\":110}},
                                {
                                    \"algTripwireEnd\":{\"algPointX\":400,\"algPointY\":410},
                                    \"algTripwireStart\":{\"algPointX\":300,\"algPointY\":310}
                                }]
                        }
                    }"


                    var objData=JSON.parse(strPEAParam)

                    id_root.setTripwiresParam(objData.algParamTripwire)
                }
            }

        }

        Row{
            Button{
                text:"getTripwiresContent"
                onClicked: {
                    var xxx =id_root.getTripwiresParam()
                    console.log(JSON.stringify(xxx))
                }
            }


            Button{
                text:"LoadTripwiresData"
                onClicked: {

                    var strPEAParam = "{
                        \"algParam\":
                        {
                            \"algParamPerimeter\":[
                                [
                                    {\"algPointX\":10,\"algPointY\":11},
                                    {\"algPointX\":20,\"algPointY\":21},
                                    {\"algPointX\":30,\"algPointY\":31},
                                    {\"algPointX\":40,\"algPointY\":41}]],
                            \"algParamTripwire\":[
                                {
                                    \"algTripwireEnd\":{\"algPointX\":200,\"algPointY\":210},
                                    \"algTripwireStart\":{\"algPointX\":100,\"algPointY\":110}},
                                {
                                    \"algTripwireEnd\":{\"algPointX\":400,\"algPointY\":410},
                                    \"algTripwireStart\":{\"algPointX\":300,\"algPointY\":310}
                                }]
                        }
                    }"


//                    strPEAParam = "{\"algParam\":{\"algParamPerimeter\":[[{\"algPointX\":10,\"algPointY\":11},{\"algPointX\":20,\"algPointY\":21},{\"algPointX\":30,\"algPointY\":31},{\"algPointX\":40,\"algPointY\":41}]],\"algParamTripwire\":[{\"algTripwireEnd\":{\"algPointX\":200,\"algPointY\":210},\"algTripwireStart\":{\"algPointX\":100,\"algPointY\":110}},{\"algTripwireEnd\":{\"algPointX\":400,\"algPointY\":410},\"algTripwireStart\":{\"algPointX\":300,\"algPointY\":310}}]}}"



                    var objData=JSON.parse(strPEAParam)

                    console.log(JSON.stringify(objData))



                    id_root.setTripwiresParam(objData.algParam.algParamTripwire)
                }
            }

            Button{
                text:"getPerimetersContent"
                onClicked: {
                    var xxx =id_root.getPerimetersParam()
                    console.log(JSON.stringify(xxx))
                }
            }


            Button{
                text:"LoadPerimetersData"
                onClicked: {
                    var strPEAParam = "{
                        \"algParam\":
                        {
                            \"algParamPerimeter\":[
                                [
                                    {\"algPointX\":10,\"algPointY\":11},
                                    {\"algPointX\":120,\"algPointY\":121},
                                    {\"algPointX\":30,\"algPointY\":31},
                                    {\"algPointX\":140,\"algPointY\":141}
                                ],
                                [
                                    {\"algPointX\":110,\"algPointY\":111},
                                    {\"algPointX\":20,\"algPointY\":21},
                                    {\"algPointX\":130,\"algPointY\":131},
                                    {\"algPointX\":40,\"algPointY\":41}
                                ],
                            ],
                            \"algParamTripwire\":[
                                {
                                    \"algTripwireEnd\":{\"algPointX\":200,\"algPointY\":210},
                                    \"algTripwireStart\":{\"algPointX\":100,\"algPointY\":110}},
                                {
                                    \"algTripwireEnd\":{\"algPointX\":400,\"algPointY\":410},
                                    \"algTripwireStart\":{\"algPointX\":300,\"algPointY\":310}
                                }]
                        }
                    }"



                    strPEAParam = "{\"algParam\":{\"algParamPerimeter\":[[{\"algPointX\":10,\"algPointY\":11},{\"algPointX\":20,\"algPointY\":21},{\"algPointX\":30,\"algPointY\":31},{\"algPointX\":40,\"algPointY\":41}],[{\"algPointX\":10,\"algPointY\":11},{\"algPointX\":20,\"algPointY\":21},{\"algPointX\":30,\"algPointY\":31},{\"algPointX\":40,\"algPointY\":41}]],\"algParamTripwire\":[{\"algTripwireEnd\":{\"algPointX\":200,\"algPointY\":210},\"algTripwireStart\":{\"algPointX\":100,\"algPointY\":110}},{\"algTripwireEnd\":{\"algPointX\":400,\"algPointY\":410},\"algTripwireStart\":{\"algPointX\":300,\"algPointY\":310}}]}}"



                    var objData=JSON.parse(strPEAParam)

                    console.log(JSON.stringify(objData))

                    id_root.setPerimetersParam(objData.algParam.algParamPerimeter)
                }
            }


        }

    }





    function clearItem(){
        id_root.__clearItems()
        currentPerimeterNum=0;
        currentTripwireNum=0;
    }

    function deleteItem(){
        var t_type = id_root.__deleteItem()
        if(t_type ===id_root.typePerimeter) currentPerimeterNum--;
        if(t_type === id_root.typeTripwire) currentTripwireNum--;
    }

    function addTripwireItem(){
        var t_points =[{x:100,y:100},{x:100,y:200}]
        var t_type =id_root.typeTripwire
        id_root.__addItem(t_type,t_points)
        id_root.currentTripwireNum++;

    }
    function setTripwiresParam(objData){

//        objData=[
//                    {
//                        "algTripwireEnd":{"algPointX":200,"algPointY":210},
//                        "algTripwireStart":{"algPointX":100,"algPointY":110}
//                    },
//                    {
//                        "algTripwireEnd":{"algPointX":200,"algPointY":210},
//                        "algTripwireStart":{"algPointX":200,"algPointY":410}
//                    },
//                ]

        var t_type = "algParamTripwire"
        var t_TripwireItems = []
        t_TripwireItems = objData
        console.log(JSON.stringify(objData))
        t_TripwireItems.forEach(function(item){
            var t_data =__convertTripwireDataPointsToDrawPoints(item)

            id_root.__addItem(t_type,t_data)
            currentTripwireNum++;
        })

    }
    function getTripwiresParam(){
        var t_drawItemList=[]
        var algParamTripwires=[]
        t_drawItemList = id_root.__getDrawItemsData()
        t_drawItemList.forEach(function(drawItem){
            if(drawItem.type === "algParamTripwire"){
                var t_Tripwire ={algParamTripwire:__convertTripwireDrawPointsToDataPoints(drawItem.data)}
                algParamTripwires.push(t_Tripwire.algParamTripwire)

            }
        })
        return algParamTripwires;
    }
    function __convertTripwireDataPointsToDrawPoints(dataPointsObj){

//   dataPointsObj= {   "algTripwireEnd":{"algPointX":200,"algPointY":210},
//                      "algTripwireStart":{"algPointX":100,"algPointY":110}}

        var draw_ps =[]
        var data_ps={}

        data_ps = dataPointsObj

        // add start point
        var t_ps =data_ps.algTripwireStart
        var v_x =  t_ps.algPointX;
        var v_y =  t_ps.algPointY;
        draw_ps.push({x:v_x,y:v_y})

        //add end point
        var t_pe =data_ps.algTripwireEnd
        v_x =  t_pe.algPointX;
        v_y =  t_pe.algPointY;
        draw_ps.push({x:v_x,y:v_y})

        return draw_ps;

    }
    function __convertTripwireDrawPointsToDataPoints(objPointList){
        var data_ps ={}
        var t_ps = objPointList[0]
        var t_pe = objPointList[1]
        data_ps.algTripwireStart ={"algPointX":t_ps.x,"algPointY":t_ps.y}
        data_ps.algTripwireEnd ={"algPointX":t_pe.x,"algPointY":t_pe.y}
        return data_ps
    }

    function addPerimeterItem(){
        var t_points =[{x:100,y:100},{x:100,y:200},{x:200,y:200},{x:200,y:100}]
        var t_type =id_root.typePerimeter
        id_root.__addItem(t_type,t_points)
        id_root.currentPerimeterNum++;

    }
    function setPerimetersParam(objData){
        var t_type = "algParamPerimeter"
        var t_PerimetersItems = []
        t_PerimetersItems = objData
        console.log(JSON.stringify(objData))
        t_PerimetersItems.forEach(function(item){
            var t_data =__convertPerimeterDataPointsToDrawPoints(item)
            id_root.__addItem(t_type,t_data)
            currentPerimeterNum++;
        })

    }
    function getPerimetersParam(){
        var t_drawItemList=[]
        var algParamPerimeters=[]
        t_drawItemList = id_root.__getDrawItemsData()
        t_drawItemList.forEach(function(drawItem){
            if(drawItem.type === "algParamPerimeter"){
                var t_Perimeter ={algParamPerimeter:__convertPerimeterDrawPointsToDataPoints(drawItem.data)}
                algParamPerimeters.push(t_Perimeter.algParamPerimeter)
            }
        })
        return algParamPerimeters;

    }
    function __convertPerimeterDataPointsToDrawPoints(objPointList){
        var draw_ps =[]
        var data_ps =[]

        if(objPointList === null) return draw_ps
        data_ps =objPointList

        console.log(JSON.stringify(data_ps))

        data_ps.forEach(function(p){
            var v_x =  p.algPointX;
            var v_y =  p.algPointY;
            draw_ps.push({x:v_x,y:v_y})

        })
        return draw_ps

    }
    function __convertPerimeterDrawPointsToDataPoints(objPointList){

        var data_ps =[]
        objPointList.forEach(function(point){
            var r_x = point.x
            var r_y = point.y
            data_ps.push({algPointX:r_x,algPointY:r_y})

        });
        return data_ps
    }



    function getPEAData(){
        var t_algParamPerimeters= id_root.getPerimetersParam();
        var t_algParamTripwires = id_root.getTripwiresParam();

        console.debug("<<<<ParamPerimeters:"+JSON.stringify(t_algParamPerimeters));
        console.debug("<<<<ParamTripwires :"+JSON.stringify(t_algParamTripwires));
        var data ={
            algParamPerimeters:t_algParamPerimeters,
            algParamTripwires:t_algParamTripwires,
        }



        console.log(JSON.stringify(data))
        return {"algParam":data};
    }

    function setPEAData(){

    }

//        var strPEAParam = "{
//            \"algParam\":
//            {
//                \"algParamPerimeter\":[
//                    [
//                        {\"algPointX\":10,\"algPointY\":11},
//                        {\"algPointX\":20,\"algPointY\":21},
//                        {\"algPointX\":30,\"algPointY\":31},
//                        {\"algPointX\":40,\"algPointY\":41}]],
//                \"algParamTripwire\":[
//                    {
//                        \"algTripwireEnd\":{\"algPointX\":200,\"algPointY\":210},
//                        \"algTripwireStart\":{\"algPointX\":100,\"algPointY\":110}},
//                    {
//                        \"algTripwireEnd\":{\"algPointX\":400,\"algPointY\":410},
//                        \"algTripwireStart\":{\"algPointX\":300,\"algPointY\":310}
//                    }]
//            }
//        }"
}

