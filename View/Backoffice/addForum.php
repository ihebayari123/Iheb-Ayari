<?php
include '../../Controller/ForumController.php';

if (isset($_POST['title']) && isset($_POST['content']) && isset($_POST['author'])) {
    if (!empty($_POST['title']) && !empty($_POST['content']) && !empty($_POST['author'])) {
        $created_at = isset($_POST['created_at']) && !empty($_POST['created_at']) ? new DateTime($_POST['created_at']) : null;

        $post = new Post(
            null,
            $_POST['title'],
            $_POST['content'],
            $created_at,
            $_POST['author']
        );

        $postC = new PostController();
        $postC->addPost($post);

        header('Location: ../../indexForum.php');
    }
}
?>



<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Ajouter un Post</title>
</head>
<body>
    <form action="" method="POST">
        <label for=""> Title </label>
        <input type="text" name="title">

        <label for=""> Content </label>
        <textarea name="content"></textarea>
        
        <label for="">Created At</label>
        <input type="datetime-local" name="created_at">

        <label for="">Author</label>
        <input type="text" name="author">

        <input type="submit" value="OK">
    </form>
</body>
</html>
