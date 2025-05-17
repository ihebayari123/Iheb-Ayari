<?php
    include '../../Controller/ForumController.php';

    $postC = new PostController();
    $postC->deletePost($_GET['id']);
    
    // Redirect to index.php with the #index anchor
    header('Location: index.php#intro');
    exit();
?>
