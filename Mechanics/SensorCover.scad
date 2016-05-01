/*cube([34,26.5,2.5]);

translate([-1,-1,-2])
{
cube([36,28.5,5]);
}*/

difference()
{
    cube([36,28.5,5]);
    
    translate([1,1,0])
    {
      cube([34,26.5,5]);
    }  
}

difference()
{

translate([0,9.25,5])
{
cube([36,10,1]);
}

translate([18,14.25,4])
{
    cylinder(10,5,5);
}

}

translate([14.25,19.25,5])
{
cube([7.5,9.25,7]);
}


translate([14.25,7.5,5])
{
cube([7.5,2,1]);
}

difference()
{

translate([18,14.25,6])
{
    cylinder(6,6.5,6.5);
}

translate([18,14.25,6])
{
    cylinder(10,5,5);
}

}

translate([-25,0,-30.5])
{
    cube([60,28.5,1.5]);
}

translate([-25,0,-30])
{
    cube([1.5,28.5,40]);
}

translate([0,0,-30])
{
    cube([1,5,30.5]);
}

translate([0,23.5,-30])
{
    cube([1,5,30.5]);
}

translate([35,0,-30.5])
{
    cube([1,5,30.5]);
}

translate([35,23.5,-30.5])
{
    cube([1,5,30.5]);
}