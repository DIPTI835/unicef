<?php
session_start();
if(!isset($_SESSION['company_name'])){
    header("Location: login.php");
    exit();
}


// Redirect directly to manufacture.cgi
header("Location: /cgi-bin/manufacture.cgi");
exit();
?>
