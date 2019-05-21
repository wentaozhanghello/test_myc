void stm32_control_eval_led_treat(void)
{
	int err;
	
	DbgFuncEntry();
#if 1
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	err = pthread_create(&control_eval_led_thread_id, &attr, stm32_control_eval_led, NULL);

    if(err != 0)
	{
    	DbgError("control eval led thread error: %s/n",strerror(err));  
		return;
	}
	
	pthread_attr_destroy(&attr); /*不再使用线程属性，将其销毁*/
#else
	err = pthread_create(&control_eval_led_thread_id, NULL, stm32_control_eval_led, NULL);
    if(err != 0)
	{
    	DbgError("control eval led thread error: %s/n",strerror(err));  
		return;
	}
#endif
	DbgFuncExit();
}