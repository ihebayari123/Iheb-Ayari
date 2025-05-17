<?php
include '../../Controller/ForumController.php';

if (isset($_POST['reaction'], $_POST['comment_id'])) {
    $reaction = $_POST['reaction'];
    $commentId = (int)$_POST['comment_id'];
    $commentController = new CommentController();
    
    if ($reaction === 'like') {
        $commentController->updateLikes($commentId, true);
    } elseif ($reaction === 'dislike') {
        $commentController->updateLikes($commentId, false);
    }
}

header("Location: commentList.php?post_id=" . $_GET['post_id']);
exit();
