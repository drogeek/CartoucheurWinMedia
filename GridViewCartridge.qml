import QtQuick 2.7

GridView {
    id:root
    property int oldX
    property int oldY
    property int xIndex
    property int yIndex
    signal dimensionChanged(int xIndex, int yIndex)
    onWidthChanged: function(){
        if(atYEnd){
            oldX = xIndex
            oldY = yIndex
            xIndex=root.width/root.cellWidth
            yIndex=root.height/root.cellHeight
            if(xIndex != 0 && yIndex !=0 && (xIndex!=oldX || yIndex!=oldY)){
                root.dimensionChanged(xIndex,yIndex)
            }
        }
    }

    Connections{
        onDimensionChanged: root.model.fitToDimension(xIndex,yIndex)
    }
}
