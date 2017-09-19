/*
 * Copyright 2017 sajith kamalnath  sajithkamal@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef  SRTOS_H
#define  SRTOS_H

#include "skernel.h"
#define s_delay(delay_count) {  s_function( s_delay__, delay_count);}
void s_mutex_init( struct s_mutex *mutex );
int s_mutex_unlock(struct s_mutex *mutex);
#define s_mutex_lock(mutex_ptr, timeout, ret_ptr) {s_function(s_mutex_lock__, mutex_ptr, timeout, ret_ptr );}
#define s_thread_yield  s_yield


void init_srtos(void);

void* create_sthread(unsigned char *stack,  int delay, int priotity, thread_entry_t entry );
void main_app( int k );
#endif
