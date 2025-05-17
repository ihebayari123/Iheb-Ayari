<?php
include '../../Controller/ForumController.php';

session_start();

if (!isset($_SESSION['user_name'])) {
    // Redirect to the signin page if the user is not logged in
    header('Location: signin.php');
    exit();
}

if (isset($_POST['title']) && isset($_POST['content']) && isset($_POST['author'])) {
    if (!empty($_POST['title']) && !empty($_POST['content']) && !empty($_POST['author'])) {
        $created_at = isset($_POST['created_at']) && !empty($_POST['created_at']) ? new DateTime($_POST['created_at']) : null;
        $author = $_SESSION['user_name']; // Logged-in user's name
        $post = new Post(
            null,
            $_POST['title'],
            $_POST['content'],
            $created_at,
            $author,
            null
        );

        $postC = new PostController();
        $postC->addPost($post);

        header('Location: index.php#intro'); // Redirect back after posting

        exit;
    }
}
?>
