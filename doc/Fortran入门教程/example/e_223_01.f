 ! F90的固定格式 －－－－－统计学生考试合格人数 
      INTEGER :: n, s, k
      s = 0; k = 0
    1 PRINT*,'键入分数 （负数时退出）：'
      READ*, n;   IF(n < 0) GOTO 999
      s = s + 1
      IF(n >= 60) k = k + 1
      GOTO 1
  999 PRINT*, k,' 个学生合格。 总人数＝', s
      END