<?php
session_start();

if (isset($_SESSION['last_activity']) && (time() - $_SESSION['last_activity'] > 1800)) {
    session_unset();
    session_destroy();
    header("Location: signin.php");
    exit();
}
$_SESSION['last_activity'] = time();
?>
