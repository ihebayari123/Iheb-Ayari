<?php
    include"../../Controller/ProductController.php";

    $Product = new ProductController();
    $Product->deleteProduct($_GET['id']);
    header ('Location:produitList.php');
    ?> 