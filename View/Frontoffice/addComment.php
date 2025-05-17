<?php
include '../../Controller/ForumController.php';
session_start();
header('Content-Type: application/json'); // Ensure the response is JSON

if (
    isset($_POST['post_id']) && 
    isset($_POST['commentContent']) && isset($_POST['commentAuthor'])
) {
    if (
        !empty($_POST['post_id']) && 
        !empty($_POST['commentContent']) && !empty($_POST['commentAuthor'])
    ) {
        $nom = $_SESSION['user_name'];

        // Create the comment object
        $comment = new Comment(
            null, // ID will be auto-generated
            $_POST['commentContent'], 
            $_POST['post_id'], // Post ID to link this comment to a specific post
            null, // `created_at` will be set automatically
            $nom
        );

        // Instantiate the CommentController
        $commentC = new CommentController();
        
        // Add the comment to the database
        $commentC->addComment($comment);

        // Return a success response
        echo json_encode([
            'status' => 'success',
            'message' => 'Comment added successfully',
            'comment' => [
                'author' => $nom,
                'content' => $_POST['commentContent'],
                'created_at' => date('Y-m-d H:i:s') // Add current timestamp for display
            ]
        ]);
    } else {
        // Return an error response for missing fields
        echo json_encode(['status' => 'error', 'message' => 'All fields are required.']);
    }
} else {
    // Return an error response for invalid submission
    echo json_encode(['status' => 'error', 'message' => 'Invalid request.']);
}
?>
