<?php
include '../../Controller/ForumController.php';

$postC = new PostController();

// Fetch search query if provided, otherwise fetch all posts
$searchText = isset($_GET['text']) ? trim($_GET['text']) : '';

// Fetch posts based on search query
$list = $postC->listPosts(1, PHP_INT_MAX, $searchText);

if (empty($list)) {
    echo '<p>No posts found.</p>';
} else {
    foreach ($list as $post) {
        echo '
            <div class="card">
                <div class="body">
                    <h2 class="title">' . htmlspecialchars($post['title']) . '</h2>
                    <p class="text">' . htmlspecialchars($post['content']) . '</p>
                    <span class="username">from: ' . htmlspecialchars($post['author']) . '</span>
                    <span class="created_at">Created at: ' . htmlspecialchars($post['created_at']) . '</span>
                    <div class="actions">
                        <a href="deleteForum.php?id=' . htmlspecialchars($post['id']) . '">Supprimer</a> |
                        <a id="modifierLink" href="updateForum.php?id=' . htmlspecialchars($post['id']) . '">Modifier</a>
                    </div>
                </div>
            </div>';
    }
}
?>
