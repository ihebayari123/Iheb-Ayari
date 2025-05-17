<?php
include '../../Controller/ForumController.php';
session_start();
$postC = new PostController();
$nom = $_SESSION['user_name'] ?? null;
$id = $_SESSION['user_id'] ?? null;
if (!isset($_GET['id']) || empty($_GET['id'])) {
    die('Error: Post ID is missing.');
}

$post = $postC->getPost($_GET['id']); // Fetch existing post

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['title'], $_POST['content'], $_POST['author']) 
        && !empty($_POST['title']) 
        && !empty($_POST['content']) 
        && !empty($_POST['author'])) {
        
        // Create Post object for update
        $post = new Post(
            $_GET['id'],
            $_POST['title'],
            $_POST['content'],
            null, // Not updating created_at
            $nom,
            $id
        );

        $postC->updatePost($post);

        // Redirect after successful update
        echo "<script>window.location.href = 'index.php#intro';</script>";
        exit;
    } else {
        echo 'Error: All fields are required.';
    }
}

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Update Post</title>
    <link rel="stylesheet" href="style.css">
    <link rel="stylesheet" href="assets/css/main.css">
</head>
<body>


<div id="bg"></div>
<form action="updateForum.php?id=<?php echo $_GET['id']; ?>" method="POST" class="update-container" onsubmit="return validateForm()">
    <label class="title" for="title">Title</label>
    <input class="input" type="text" name="title" value="<?php echo htmlspecialchars($post['title']); ?>" >

    <label class="content" for="content">Content</label>
    <textarea class="input" name="content" ><?php echo htmlspecialchars($post['content']); ?></textarea>

    <label class="author" for="author">Author</label>
    <input class="input" type="text" name="author" value="<?php echo isset($_SESSION['user_name']) ? htmlspecialchars($_SESSION['user_name']) : ''; ?>" readonly >

    <button class="button type1">
        <span class="btn-txt">update</span>
    </button>
</form>
<script src="work.js"></script>
</body>


</html>

