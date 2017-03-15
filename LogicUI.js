var oldX = 0
var oldY = 0
var xIndex
var yIndex
function computeDimension() {
    oldX = xIndex
    oldY = yIndex
    console.log("width:"+grid.width)
    console.log("cellWidth:"+grid.cellWidth)
    console.log("height:"+grid.height)
    console.log("cellHeight:"+grid.cellHeight)
    xIndex=Math.floor(grid.width/grid.cellWidth)
    yIndex=Math.floor(grid.height/grid.cellHeight)
    console.log("xIndex:"+xIndex)
    console.log("yIndex:"+yIndex)
    if(xIndex !== 0 && yIndex !==0 && (xIndex!==oldX || yIndex!==oldY)){
        grid.dimensionChanged(xIndex,yIndex)
    }

}
