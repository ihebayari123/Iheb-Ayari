<?php
include '../../Controller/ForumController.php';

$postC = new PostController();

if (isset($_POST['title']) && isset($_POST['content']) && isset($_POST['created_at']) && isset($_POST['author'])) {
    if (!empty($_POST['title']) && !empty($_POST['content']) && !empty($_POST['created_at']) && !empty($_POST['author'])) {
        $post = new Post(
            $_GET['id'],
            $_POST['title'],
            $_POST['content'],
            new DateTime($_POST['created_at']),
            $_POST['author']
        );

        $postC->updatePost($post);

        header('Location: ../Forum.html');
        exit();
    }
}

// Fetch the post details for the given ID
$post = $postC->getPost($_GET['id']);
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Modifier le Post</title>
    <link rel="stylesheet" href="styles.css">
</head>
<body>
    <header>
        <h1>Modifier le Post</h1>
    </header>
    <div class="container">
        <form action="" method="POST">
            <label for="title"> Title </label>
            <input type="text" name="title" value="<?php echo htmlspecialchars($post['title']); ?>">

            <label for="content"> Content </label>
            <textarea name="content"><?php echo htmlspecialchars($post['content']); ?></textarea>
            
            <label for="created_at">Created At</label>
            <input type="datetime-local" name="created_at" value="<?php echo date('Y-m-d\TH:i', strtotime($post['created_at'])); ?>">

            <label for="author">Author</label>
            <input type="text" name="author" value="<?php echo htmlspecialchars($post['author']); ?>">

            <input type="submit" value="OK">
        </form>
    </div>
</body>
</html>
