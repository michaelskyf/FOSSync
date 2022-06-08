#pragma once

int worker_create(int connfd);
void worker_cleanup(int blocking);
