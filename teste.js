function ola()
{
    const num = 10;
    if(num>5)
    {
        console.log("dentro do if da function OLA");
    }
}
function soma()
{
    const num = 10;
    if(num>5)
    {
        if(num < 20)
        {
            console.log("dentro da function SOMA");
        }
    }
}
console.log("ola guilherme fora da function");
soma();
ola();

