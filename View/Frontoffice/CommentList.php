<?php
include '../../Controller/ForumController.php';
session_start();
$postId = isset($_GET['post_id']) ? (int)$_GET['post_id'] : 0;

if ($postId <= 0) {
    echo "Invalid Post ID.";
    exit;
}

$commentController = new CommentController();
$comments = $commentController->listComments($postId);
?>

<div class="comment-list">
    <?php if (empty($comments)) { ?>
        <p>No comments yet. Be the first to comment!</p>
    <?php } else { ?>
        <?php foreach ($comments as $comment) { ?>
            <div class="comment-card">
                <p><strong><?php echo htmlspecialchars($comment['author']); ?></strong>: <?php echo htmlspecialchars($comment['content']); ?></p>
                <span class="comment-date"><?php echo htmlspecialchars($comment['created_at']); ?></span>
            </div>
        <?php } ?>
    <?php } ?>
    

    <!-- Add Comment Form -->
    <form action="addComment.php" method="POST" class="add-comment-form">
        <input type="hidden" name="post_id" value="<?php echo $postId; ?>">
        <textarea name="commentContent" placeholder="Write a comment..." required></textarea>
        <input type="text" name="commentAuthor" value="<?php echo isset($_SESSION['user_name']) ? htmlspecialchars($_SESSION['user_name']) : ''; ?>" readonly>
        <button type="submit">Post Comment</button>
    </form>
</div>
