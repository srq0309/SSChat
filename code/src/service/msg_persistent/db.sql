-- 用户表
CREATE TABLE ssim_user (
    username CHAR (16)  PRIMARY KEY
                        NOT NULL,
    password CHAR (32)  NOT NULL,
    question CHAR (512) NOT NULL,
    answer   CHAR (512) NOT NULL
);

-- 离线消息表
CREATE TABLE ssim_msg (
    username CHAR (16) NOT NULL,
    message  BLOB      NOT NULL
);