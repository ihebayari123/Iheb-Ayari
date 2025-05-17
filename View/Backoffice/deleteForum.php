<?php
include '../../Controller/ForumController.php';

if (isset($_GET['id'])) {
    $id = intval($_GET['id']);
    $postC = new PostController();

    // Call the delete function
    $postC->deletePost($id);

    // Redirect back to the forum page
    header("Location: Forum.php");
    exit();
} else {
    echo "Invalid post ID";
}
?>
