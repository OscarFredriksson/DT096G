
class Iterator
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
        return this.arr[this.index++];
    }
}

Array.prototype.iterator = function(){return new Iterator(this)};

var ar = new Array("Hej", "Då", "rå");

var it = ar.iterator();

while(it.hasNext())
{
    console.log(it.next());
}