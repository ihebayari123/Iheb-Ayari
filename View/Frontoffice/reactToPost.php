<?php
include '../../Controller/ForumController.php';
session_start();
header('Content-Type: application/json');

if (isset($_POST['post_id']) && isset($_POST['reaction_type'])) {
    $postId = $_POST['post_id'];
    $reactionType = $_POST['reaction_type'];
    $userId = $_SESSION['user_id']; // Ensure user is logged in and ID is available

    if (!in_array($reactionType, ['like', 'dislike'])) {
        echo json_encode(['status' => 'error', 'message' => 'Invalid reaction type']);
        exit();
    }

    // Instantiate the ForumController
    $forumController = new ForumController();

    // Check if the user already reacted
    $existingReaction = $forumController->getUserReaction($postId, $userId);

    if ($existingReaction) {
        if ($existingReaction['reaction_type'] === $reactionType) {
            // If the same reaction is sent, remove it (toggle)
            $forumController->removeReaction($postId, $userId);
            echo json_encode(['status' => 'success', 'message' => 'Reaction removed']);
        } else {
            // Update the reaction to the opposite type
            $forumController->updateReaction($postId, $userId, $reactionType);
            echo json_encode(['status' => 'success', 'message' => 'Reaction updated']);
        }
    } else {
        // Add a new reaction
        $forumController->addReaction($postId, $userId, $reactionType);
        echo json_encode(['status' => 'success', 'message' => 'Reaction added']);
    }
} else {
    echo json_encode(['status' => 'error', 'message' => 'Invalid request']);
}
?>
