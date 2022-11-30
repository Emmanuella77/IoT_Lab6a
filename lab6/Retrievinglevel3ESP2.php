<?php
    $servername = "localhost";
    $username = "root";
    $password = "";
    $dbname = "lab6";

    $con = mysqli_connect($servername,$username,$password,$dbname);
    
    $data=array(); 
 

   $sql = "SELECT * FROM (
    SELECT * 
    FROM data_sensors 
    WHERE (ESP_ID = 1)  ORDER BY ID DESC LIMIT 5
  ) AS `table` ORDER by ID ASC";

//check what ID is 

   if ($q=mysqli_query($con, $sql)) {
    while ($row = $q->fetch_assoc()){
        echo "{$row['Distance']}  {$row['Timeread']} \n";
    }
   } 
   
?>
