// main.c

#include <stdio.h>
#include "url_generator.h"
#include "redis_handler.h"
#include "utils.h"

int main(int argc, char const *argv[])
{
    redisContext *conn = connect_redis("127.0.0.1", 6379);

    if (conn == NULL)
    {
        printf("连接失败，请重试！\n");
        return 1;
    }

    printf("连接成功！\n");
    printf("欢迎使用短地址生成器！\n");

    while (1)
    {
        printf("================TINY URL 2.0(redis)=================\n");
        printf("____________________1. 生成短地址____________________\n");
        printf("____________________2. 解析短地址____________________\n");
        printf("____________________3. 延长有效期______________________\n");
        printf("____________________4. 增加点数______________________\n");
        printf("____________________5. 显示数据______________________\n");
        printf("____________________6. 统计信息______________________\n");
        printf("____________________7. 退出程序______________________\n");

        int choice = get_choice();
        switch (choice)
        {
        case 1:
            create_url(conn);
            break;
        case 2:
            select_url(conn);
            break;
        case 3:
            extend_time(conn);
            break;
        case 4:
            incr_count_user(conn);
            break;
        case 5:
            show_all(conn);
            break;
        case 6:
            display_all(conn);
            break;
        case 7:
            printf("再见！\n");
            redisFree(conn);
            return 0;
        default:
            printf("无效的选择，请重试。\n");
            break;
        }
    }
    redisFree(conn);
    return 0;
}
