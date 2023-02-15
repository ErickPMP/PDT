float Out2
if(t<10)
{
  Out2 = 10*t;
}
else if(t>10 && t<=20)
{
  Out2 = 100;
}
else if(t>20 && t<=25)
{
  Out2 = 10*t - 100;
}
else if(t>25 && t<=30)
{
  Out2 = 150;
}
else if(t>30 && t<=40)
{
  Out2 = -15*t + 600;
}
else if(t>40)
{
  t = 0;
  mil = 0;
  sec = 0;
}
