
class Iterable
{
    index = 0;

    constructor(arr)
    {
        this.arr = arr;
    }

    hasNext()
    {
        return this.index < this.arr.length;
    }

    next()
    {
        //this.index++
        return this.arr[this.index++];
    }
}

Array.prototype.iterator = function(){return new Iterable(this)};

var ar = new Array("a", "b", "c", "d", "e", "f");

var it = ar.iterator();

while(it.hasNext())
{
    console.log(it.next());
}