<?php

include '../../Controller/ForumController.php';

$user=$_SESSION['user_id'];
$postC = new PostController();

// Check if search query is provided
$searchText = isset($_GET['text']) ? $_GET['text'] : '';

// If no search text, return all posts
$list = $postC->listPosts(1, PHP_INT_MAX, $searchText);



?>

<style>
/* Feed container */
.feed-container {
  max-width: 800px;
  margin: 0 auto;
  padding: 20px;
  font-family: "Poppins", sans-serif;
  color: #333;
}

/* Post card */
.post-card {
  background: #ffffff;
  margin-bottom: 20px;
  padding: 20px;
  border: 1px solid #e0e0e0;
  border-radius: 10px;
  box-shadow: 0 4px 10px rgba(0, 0, 0, 0.05);
  transition: transform 0.3s ease, box-shadow 0.3s ease;
  position: relative;
}

.post-card:hover {
  transform: translateY(-5px);
  box-shadow: 0 6px 15px rgba(0, 0, 0, 0.1);
}

/* User info */
.post-header {
  display: flex;
  align-items: center;
  gap: 15px;
  margin-bottom: 15px;
}

.user-avatar {
  width: 50px;
  height: 50px;
  border-radius: 50%;
  overflow: hidden;
  display: flex;
  align-items: center;
  justify-content: center;
  background: #f0f0f0;
}

.user-avatar svg {
  width: 30px;
  height: 30px;
  fill: #7cdacc;
}

.user-info {
  flex: 1;
}

.user-info h3 {
  margin: 0;
  font-size: 18px;
  color: #2cb5a0;
}

.user-info span {
  font-size: 14px;
  color: #777;
}

/* Post content */
.post-content {
  font-size: 16px;
  line-height: 1.6;
  color: #555;
  margin-bottom: 15px;
  word-wrap: break-word; /* Ensure long words break to the next line */
  overflow: hidden; /* Prevent content from overflowing */
  text-overflow: ellipsis; /* Add ellipsis (...) for overflowing text */
}

/* Action buttons */
.post-actions {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-top: 10px;
}

.action-icons a {
  text-decoration: none;
  margin-right: 15px;
  position: relative;
  color: #666;
  transition: color 0.2s ease;
}

.action-icons a:hover {
  color: #007bff;
}

/* Tooltip styling */
.action-icons a .tooltip {
  display: none;
  position: absolute;
  bottom: -25px;
  left: 50%;
  transform: translateX(-50%);
  padding: 5px 8px;
  background: #333;
  color: #fff;
  font-size: 12px;
  border-radius: 5px;
  white-space: nowrap;
  z-index: 10;
}

.action-icons a:hover .tooltip {
  display: block;
}

/* Date created */
.date-created {
  font-size: 12px;
  color: #999;
  text-align: right;
}

/* Responsive */
@media (max-width: 768px) {
  .post-card {
    padding: 15px;
  }

  .post-content {
    font-size: 14px;
  }
}

.comment-list {
    padding: 10px;
    background: #f9f9f9;
    border: 1px solid #e0e0e0;
    border-radius: 10px;
    margin-top: 10px;
}

.comment-card {
    padding: 10px;
    margin-bottom: 10px;
    background: #ffffff;
    border: 1px solid #ddd;
    border-radius: 5px;
}

.comment-card strong {
    color: #007bff;
}

.comment-card .comment-date {
    font-size: 12px;
    color: #999;
    display: block;
    margin-top: 5px;
}

.add-comment-form {
    margin-top: 15px;
    display: flex;
    flex-direction: column;
    gap: 10px;
}

.add-comment-form textarea {
    resize: none;
    padding: 10px;
    border: 1px solid #ccc;
    border-radius: 5px;
}

.add-comment-form button {
    align-self: flex-end;
    padding: 8px 12px;
    background: #007bff;
    color: #fff;
    border: none;
    border-radius: 5px;
    cursor: pointer;
}

.add-comment-form button:hover {
    background: #0056b3;
}

.reaction-btn {
    background: #f0f0f0;
    padding: 8px 12px;
    border: 1px solid #ccc;
    border-radius: 5px;
    cursor: pointer;
    transition: background 0.2s ease;
}

.reaction-btn:hover {
    background: #e0e0e0;
}

.reaction-btn.liked {
    color: #007bff; /* Blue for liked */
}

.reaction-btn.disliked {
    color: #d9534f; /* Red for disliked */
}


</style>
<head>
    <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css" rel="stylesheet">
</head>

<!-- Feed container -->
<div class="feed-container">
    <?php foreach ($list as $post) { ?>
        <div class="post-card">
            <!-- Post Header -->
            <div class="post-header">
                <div class="user-avatar">
                    <svg viewBox="0 0 448 512" xmlns="http://www.w3.org/2000/svg">
                        <path d="M224 256c70.7 0 128-57.31 128-128s-57.3-128-128-128C153.3 0 96 57.31 96 128S153.3 256 224 256zM274.7 304H173.3C77.61 304 0 381.6 0 477.3c0 19.14 15.52 34.67 34.66 34.67h378.7C432.5 512 448 496.5 448 477.3C448 381.6 370.4 304 274.7 304z"></path>
                    </svg>
                </div>
                <div class="user-info">
                    <h3><?php echo htmlspecialchars($post['title']); ?></h3>
                    <span>By <?php echo htmlspecialchars($post['author']); ?></span>
                </div>
            </div>

            <!-- Post Content -->
            <p class="post-content"><?php echo htmlspecialchars($post['content']); ?></p>
            <!-- Add action buttons for reactions with dynamic classes -->
<div class="post-reactions">
    <button class="reaction-btn like-btn <?php echo $liked; ?>" data-post-id="<?php echo $post['id']; ?>" data-reaction-type="like">
        <i class="fas fa-thumbs-up"></i> Like
    </button>
    <button class="reaction-btn dislike-btn <?php echo $disliked; ?>" data-post-id="<?php echo $post['id']; ?>" data-reaction-type="dislike">
        <i class="fas fa-thumbs-down"></i> Dislike
    </button>
</div>
            <!-- Actions -->
            <div class="post-actions">
                <div class="action-icons">
                    <a href="deleteForum.php?id=<?php echo urlencode($post['id']); ?>" title="Delete">
                        <i class="fas fa-trash-alt"></i>
                    </a>
                    <a href="updateForum.php?id=<?php echo urlencode($post['id']); ?>" title="Edit">
                        <i class="fas fa-edit"></i>
                    </a>
                </div>
                <p class="date-created"><?php echo htmlspecialchars($post['created_at']); ?></p>
            </div>
            
            <!-- Show Comments Section -->
            <div class="comments-section" id="comments-<?php echo $post['id']; ?>">
                <button class="show-comments-btn" data-post-id="<?php echo $post['id']; ?>">
                    Show Comments
                </button>
                <div class="comments-container" style="display: none;">
                    <!-- Comments will be loaded here dynamically -->
                </div>
            </div>
        </div>
    <?php } ?>
</div>

<script>
   document.addEventListener('DOMContentLoaded', function () {
    const showCommentsButtons = document.querySelectorAll('.show-comments-btn');

    showCommentsButtons.forEach(button => {
        button.addEventListener('click', function () {
            const postId = button.getAttribute('data-post-id');
            const commentsContainer = button.nextElementSibling;

            if (commentsContainer.style.display === 'none' || !commentsContainer.innerHTML.trim()) {
                // Fetch comments via AJAX
                fetch(`CommentList.php?post_id=${postId}`)
                    .then(response => response.text())
                    .then(html => {
                        commentsContainer.innerHTML = html;
                        commentsContainer.style.display = 'block';

                        // Add event listener to the comment form
                        const commentForm = commentsContainer.querySelector('.add-comment-form');
                        if (commentForm) {
                            commentForm.addEventListener('submit', function (e) {
                                e.preventDefault(); // Prevent default form submission
                                const formData = new FormData(commentForm);

                                // Submit comment via AJAX
                                fetch('addComment.php', {
                                    method: 'POST',
                                    body: formData
                                })
                                    .then(response => response.json())
                                    .then(data => {
                                        if (data.status === 'success') {
                                            // Append the new comment to the comments list
                                            const commentList = commentsContainer.querySelector('.comment-list');
                                            const newComment = document.createElement('div');
                                            newComment.className = 'comment-card';
                                            newComment.innerHTML = `
                                                <p><strong>${data.comment.author}</strong>: ${data.comment.content}</p>
                                                <span class="comment-date">${data.comment.created_at}</span>
                                            `;
                                            commentList.appendChild(newComment);

                                            // Clear the form
                                            commentForm.reset();
                                        } else {
                                            alert(data.message);
                                        }
                                    })
                                    .catch(error => console.error('Error adding comment:', error));
                            });
                        }
                    })
                    .catch(error => console.error('Error loading comments:', error));
            } else {
                // Toggle comments visibility
                commentsContainer.style.display = 'none';
            }
        });
    });
});

</script>