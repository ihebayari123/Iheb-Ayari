<?php

// Post Class
class Post {
    private ?int $id;
    private ?string $title;
    private ?string $content;
    private ?DateTime $createdAt;
    private ?string $author;
    private ?int $user_id;

    public function __construct(?int $id, ?string $title, ?string $content, ?DateTime $createdAt = null, ?string $author,?int $user_id) {
        $this->id = $id;
        $this->title = $title;
        $this->content = $content;
        $this->createdAt = $createdAt ?? new DateTime();
        $this->author = $author;
        $this->user_id = $user_id;
    }

    public function getId(): ?int {
        return $this->id;
    }

    public function setId(?int $id): void {
        $this->id = $id;
    }

    public function getTitle(): ?string {
        return $this->title;
    }

    public function setTitle(?string $title): void {
        $this->title = $title;
    }

    public function getContent(): ?string {
        return $this->content;
    }

    public function setContent(?string $content): void {
        $this->content = $content;
    }

    public function getCreatedAt(): ?DateTime {
        return $this->createdAt;
    }

    public function setCreatedAt(?DateTime $createdAt): void {
        $this->createdAt = $createdAt ?? new DateTime();
    }

    public function getAuthor(): ?string {
        return $this->author;
    }

    public function setAuthor(?string $author): void {
        $this->author = $author;
    }

    public function getUserId() {
        return $this->user_id; // Return user ID
    }

    public function setUserId($user_id) {
        $this->user_id = $user_id; // Set user ID
    }
}

// Comment Class
class Comment {
    private ?int $id;
    private ?string $commentcontent;
    private ?DateTime $commentcreatedAt;
    private ?string $commentauthor;
    private ?int $postId;  // Added postId to associate comments with posts

    public function __construct(?int $id, ?string $commentcontent, ?int $postId, ?DateTime $commentcreatedAt = null, ?string $commentauthor) {
        $this->id = $id;
        $this->commentcontent = $commentcontent;
        $this->postId = $postId;
        $this->commentcreatedAt = $commentcreatedAt ?? new DateTime();
        $this->commentauthor = $commentauthor;
    }

    public function getId(): ?int {
        return $this->id;
    }

    public function setId(?int $id): void {
        $this->id = $id;
    }



    public function getcommentContent(): ?string {
        return $this->commentcontent;
    }

    public function setcommentContent(?string $commentcontent): void {
        $this->commentcontent = $commentcontent;
    }

    public function getcommentCreatedAt(): ?DateTime {
        return $this->commentcreatedAt;
    }

    public function setcommentCreatedAt(?DateTime $commentcreatedAt): void {
        $this->commentcreatedAt = $commentcreatedAt ?? new DateTime();
    }

    public function getcommentAuthor(): ?string {
        return $this->commentauthor;
    }

    public function setcommentAuthor(?string $commentauthor): void {
        $this->commentauthor = $commentauthor;
    }

    public function getPostId(): ?int {
        return $this->postId;
    }

    public function setPostId(?int $postId): void {
        $this->postId = $postId;
    }
}

class PostReactions {
    private ?int $id;
    private ?int $postId;
    private ?int $userId;
    private ?string $reactionType; // 'like' or 'dislike'
    private ?DateTime $createdAt;

    public function __construct(?int $id, ?int $postId, ?int $userId, ?string $reactionType, ?DateTime $createdAt = null) {
        $this->id = $id;
        $this->postId = $postId;
        $this->userId = $userId;
        $this->reactionType = $reactionType;
        $this->createdAt = $createdAt ?? new DateTime();
    }

    public function getId(): ?int {
        return $this->id;
    }

    public function getPostId(): ?int {
        return $this->postId;
    }

    public function setPostId(?int $postId): void {
        $this->postId = $postId;
    }

    public function getUserId(): ?int {
        return $this->userId;
    }

    public function setUserId(?int $userId): void {
        $this->userId = $userId;
    }

    public function getReactionType(): ?string {
        return $this->reactionType;
    }

    public function setReactionType(?string $reactionType): void {
        $this->reactionType = $reactionType;
    }

    public function getCreatedAt(): ?DateTime {
        return $this->createdAt;
    }
}


?>
