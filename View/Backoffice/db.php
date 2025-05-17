<?php
// db.php
$host = 'localhost';
$dbname = 'projetweb';
$username = 'root';
$password = '';

try {
    // Create a PDO instance
    $db = new PDO("mysql:host=$host;dbname=$dbname", $username, $password);
    $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION); // Optional: For error handling
} catch (PDOException $e) {
    echo "Connection failed: " . $e->getMessage();
}
