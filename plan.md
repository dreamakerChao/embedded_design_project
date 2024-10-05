```mermaid
gantt
    title Development Plan
    dateFormat  MM-DD
		axisFormat  %m-%d	

    section hardware
        sensors: work,10-12,14d
        fpga_design:work,10-23,14d
        rapi_design:work,10-15,24d
    section software
        data_collection: work,10-04,2d
        AI_model_training: work,10-04,7d
        AI_model_testing: work,10-07,6d
        The whole flow: work,11-02,14d
    
    section test
        overall_test: work,11-10,14d
        improvement:work,11-20,8d
		
  
	todayMarker off
```