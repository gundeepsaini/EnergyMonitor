
/*
	HTML Page, test here: https://www.w3schools.com/html/tryit.asp?filename=tryhtml_table_intro
	Convert here: http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html#
*/



void Prep_webpage1()
{

  String timeStr = timeClient.getFormattedTime();

  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int DayToday = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;
  
  int DayToday1 = day(epochTime) ;
  int currentMonth1 = month(epochTime);
  int currentYear1 = year(epochTime);
  

    // !!! ELements that don't change !!!
  String page_head ="<!DOCTYPE html><html><head><style>td,th{ border: 1px solid #dddddd; text-align: left; padding: 8px;} tr:nth-child(even){background-color: #dddddd;}</style></head>";
  String body_start = "<body>";
  String body_table_start = String("<h2>") + DeviceHostName + String("</h2><h4>Updated: ") + String(timeStr) + String("</h4><table>");
  String body_table_row00 ="<tr><th>#</th><th>Description</th><th>Value</th><th>Unit</th></tr>";
  String body_end = "</table></body></html>";

  // Start chaning here onwards.....

  String body_table_row01 =String("<tr><td>01</td><td>Voltage avg</td><td>")        + String(v_avg)       + String("</td><td>V</td></tr>");
  String body_table_row02 =String("<tr><td>02</td><td>Current sum</td><td>")        + String(i_sum)       + String("</td><td>A</td></tr>");
  String body_table_row03 =String("<tr><td>03</td><td>Power sum</td><td>")          + String(p_sum)       + String("</td><td>W</td></tr>");
  String body_table_row04 =String("<tr><td>04</td><td>EnergyToday sum</td><td>")    + String(e_sum_Today) + String("</td><td>kWh</td></tr>");
  String body_table_row05 =String("<tr><td>05</td><td>EnergyThisMonth sum</td><td>")+ String(e_sum_ThisMonth) + String("</td><td>kWh</td></tr>");
  String body_table_row06 =String("<tr><td>06</td><td>EnergyAllTime sum</td><td>")  + String(e1+e2+e3)    + String("</td><td>kWh</td></tr>");
  String body_table_row07 =String("<tr><td>07</td><td>PowerFactor avg</td><td>")    + String(pf_avg)      + String("</td><td>-</td></tr>");
  String body_table_row08 =String("<tr><td>08</td><td>Frequency avg</td><td>")      + String(f_avg)       + String("</td><td>hz</td></tr>");

  String body_table_row09 =String("<tr><td>09</td><td>Voltage P1</td><td>")         + String(v1)          + String("</td><td>V</td></tr>");
  String body_table_row10 =String("<tr><td>10</td><td>Voltage P2</td><td>")         + String(v2)          + String("</td><td>V</td></tr>");
  String body_table_row11 =String("<tr><td>11</td><td>Voltage P3</td><td>")         + String(v3)          + String("</td><td>V</td></tr>");  
  String body_table_row12 =String("<tr><td>12</td><td>Current P1</td><td>")         + String(i1)          + String("</td><td>A</td></tr>");
  String body_table_row13 =String("<tr><td>13</td><td>Current P2</td><td>")         + String(i2)          + String("</td><td>A</td></tr>");
  String body_table_row14 =String("<tr><td>14</td><td>Current P3</td><td>")         + String(i3)          + String("</td><td>A</td></tr>");
  String body_table_row15 =String("<tr><td>15</td><td>Power P1</td><td>")           + String(p1)          + String("</td><td>W</td></tr>");
  String body_table_row16 =String("<tr><td>16</td><td>Power P2</td><td>")           + String(p2)          + String("</td><td>W</td></tr>");
  String body_table_row17 =String("<tr><td>17</td><td>Power P3</td><td>")           + String(p3)          + String("</td><td>W</td></tr>");
  String body_table_row18 =String("<tr><td>18</td><td>EnergyToday P1</td><td>")     + String(Energy.e1_Today)     + String("</td><td>kWh</td></tr>");
  String body_table_row19 =String("<tr><td>19</td><td>EnergyToday P2</td><td>")     + String(Energy.e2_Today)     + String("</td><td>kWh</td></tr>");
  String body_table_row20 =String("<tr><td>20</td><td>EnergyToday P3</td><td>")     + String(Energy.e3_Today)     + String("</td><td>kWh</td></tr>");
  String body_table_row21 =String("<tr><td>21</td><td>EnergyThisMonth P1</td><td>") + String(Energy.e1_ThisMonth) + String("</td><td>kWh</td></tr>");
  String body_table_row22 =String("<tr><td>22</td><td>EnergyThisMonth P2</td><td>") + String(Energy.e2_ThisMonth) + String("</td><td>kWh</td></tr>");
  String body_table_row23 =String("<tr><td>23</td><td>EnergyThisMonth P3</td><td>") + String(Energy.e3_ThisMonth) + String("</td><td>kWh</td></tr>");
  String body_table_row24 =String("<tr><td>24</td><td>EnergyAllTime P1</td><td>")   + String(e1)          + String("</td><td>kWh</td></tr>");
  String body_table_row25 =String("<tr><td>25</td><td>EnergyAllTime P2</td><td>")   + String(e2)          + String("</td><td>kWh</td></tr>");
  String body_table_row26 =String("<tr><td>26</td><td>EnergyAllTime P3</td><td>")   + String(e3)          + String("</td><td>kWh</td></tr>");  
  String body_table_row27 =String("<tr><td>27</td><td>PowerFactor P1</td><td>")     + String(pf1)         + String("</td><td>-</td></tr>");
  String body_table_row28 =String("<tr><td>28</td><td>PowerFactor P2</td><td>")     + String(pf2)         + String("</td><td>-</td></tr>");
  String body_table_row29 =String("<tr><td>29</td><td>PowerFactor P3</td><td>")     + String(pf3)         + String("</td><td>-</td></tr>");
  String body_table_row30 =String("<tr><td>30</td><td>Frequency P1</td><td>")       + String(f1)          + String("</td><td>hz</td></tr>");
  String body_table_row31 =String("<tr><td>31</td><td>Frequency P2</td><td>")       + String(f2)          + String("</td><td>hz</td></tr>");
  String body_table_row32 =String("<tr><td>32</td><td>Frequency P3</td><td>")       + String(f3)          + String("</td><td>hz</td></tr>");
  String body_table_row33 =String("<tr><td>33</td><td>Motion PIR</td><td>")         + String(PIR_State)   + String("</td><td>-</td></tr>");
  String body_table_row34 =String("<tr><td>34</td><td>Date</td><td>")        + String(currentYear) + "." + String(currentMonth) + "." + String(DayToday)   + String("</td><td>-</td></tr>");
  String body_table_row35 =String("<tr><td>35</td><td>Date</td><td>")        + String(currentYear1) + "." + String(currentMonth1) + "." + String(DayToday1)   + String("</td><td>-</td></tr>");
    

  webpage1 = page_head +
  			 body_start +
  			 body_table_start +
  			 body_table_row00 +
  			 body_table_row01 +
  			 body_table_row02 +
  			 body_table_row03 +
  			 body_table_row04 +
  			 body_table_row05 +
  			 body_table_row06 +
  			 body_table_row07 +
  			 body_table_row08 +
  			 body_table_row09 +
  			 body_table_row10 +
  			 body_table_row11 +
  			 body_table_row12 +
  			 body_table_row13 +
  			 body_table_row14 +
  			 body_table_row15 +
  			 body_table_row16 +
  			 body_table_row17 +
  			 body_table_row18 +
  			 body_table_row19 +
  			 body_table_row20 +
  			 body_table_row21 +
  			 body_table_row22 +
  			 body_table_row23 +
  			 body_table_row24 +
  			 body_table_row25 +
  			 body_table_row26 +
  			 body_table_row27 +
  			 body_table_row28 +
  			 body_table_row29 +
  			 body_table_row30 +
  			 body_table_row31 +
  			 body_table_row32 +
         body_table_row33 +
         body_table_row34 +
         body_table_row35 +
  			 body_end; 
         
}


/*

Source webpage:

<!DOCTYPE html>
<html>
<head>
<style>
td,th{
  border: 1px solid #dddddd;
  text-align: left;
  padding: 8px;
}
tr:nth-child(even){
  background-color: #dddddd;
}
</style>
</head>
<body>

<h2>Device Name</h2>
<h4>Updated: Time</h4>
<table>
  <tr>
    <th>#</th>
    <th>Description</th>
    <th>Value</th>
    <th>Unit</th>
  </tr>
  <tr>
    <td>1</td>
    <td>Voltage</td>
    <td>231.1</td>
    <td>V</td>
  </tr>
  <tr>
    <td>2</td>
    <td>Current</td>
    <td>0.21</td>
    <td>A</td>
  </tr>
  <tr>
    <td>3</td>
    <td>Power</td>
    <td>3212</td>
    <td>W</td>
  </tr>
  <tr>
    <td>4</td>
    <td>Energy</td>
    <td>241</td>
    <td>kWh</td>
  </tr>
  <tr>
    <td>5</td>
    <td>Frequeny</td>
    <td>50</td>
    <td>hz</td>
  </tr>
  
</table>

</body>
</html>





*/
