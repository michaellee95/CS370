package homework1;

import java.util.Random;

public class MPI_SampleProc extends MPI_Proc
{
    public MPI_SampleProc(MPI_World world, int rank)
    {
        super(world, rank);
    }

    public void exec(int argc, String argv[]) throws InterruptedException
    {
        MPI_Init(argc, argv);
        MPI_Status status = new MPI_Status();
        int req_tag = 1;
        int task_tag = 2;
        
        Random rand = new Random();

        int[] buffer = new int[1];
        int[] tasks = new int[2];
    	int[] empty = {-1, 30};
    	
        int rank = MPI_Comm_rank(MPI_COMM_WORLD);
        int size = MPI_Comm_size(MPI_COMM_WORLD);
        
        //code for generator
        if (rank == 0) {
        	//send out 30 tasks, receive request for 30 tasks
        	for (int i = 1; i <= 30; i++) {
        		tasks[0] = i;
        		MPI_Recv(buffer, 1, MPI_INT, 2, req_tag, MPI_COMM_WORLD, status);
        		MPI_Send(tasks, 2, MPI_INT, 2, task_tag, MPI_COMM_WORLD);
        	}
        	//At the end send out empty task to signal end
        	MPI_Recv(buffer, 1, MPI_INT, 2, req_tag, MPI_COMM_WORLD, status);
        	MPI_Send(empty, 2, MPI_INT, 2, task_tag, MPI_COMM_WORLD);
        }
        
        //code for Comms
        else if (rank % 2 == 0 && rank != 0 && rank != size-1) {
            while (true) {
            	//send and receive from left
            	MPI_Send(buffer, 1, MPI_INT, rank-2, req_tag, MPI_COMM_WORLD);
            	MPI_Recv(tasks, 2, MPI_INT, rank-2, task_tag, MPI_COMM_WORLD, status);
            	//break if task is empty
            	//else send task to whoever request it
            	if (tasks[0] == -1)
            		break;
            	else {
            		MPI_Recv(buffer, 1, MPI_INT, MPI_ANY_SOURCE, req_tag, MPI_COMM_WORLD, status);
            		MPI_Send(tasks, 2, MPI_INT, status.MPI_SOURCE, task_tag, MPI_COMM_WORLD);
            	}
            }
            //Send out empty task to signal end
            //send to worker before comm
            if (rank != size - 3){
            	int temp = tasks[1];
            	tasks[1] = 1;
                MPI_Recv(buffer, 1, MPI_INT, rank-1, req_tag, MPI_COMM_WORLD, status);
                MPI_Send(tasks, 2, MPI_INT, rank-1, task_tag, MPI_COMM_WORLD);
                System.out.printf("Comm %d sending energy %d to worker %d\n", rank, 1, rank-1);
                tasks[1] = temp-1;
                MPI_Recv(buffer, 1, MPI_INT, rank+2, req_tag, MPI_COMM_WORLD, status);
                MPI_Send(tasks, 2, MPI_INT, rank+2, task_tag, MPI_COMM_WORLD);
                System.out.printf("Comm %d sending energy %d to comm %d, goodbye!\n", rank, tasks[1], rank+2); 
            }  
            else{
            	int temp = tasks[1];
            	tasks[1] = 1;
                MPI_Recv(buffer, 1, MPI_INT, rank-1, req_tag, MPI_COMM_WORLD, status);
                MPI_Send(tasks, 2, MPI_INT, rank-1, task_tag, MPI_COMM_WORLD);
                System.out.printf("Comm %d sending energy %d to worker %d\n", rank, 1, rank-1);  
                tasks[1] = temp-1;
                MPI_Recv(buffer, 1, MPI_INT, rank+1, req_tag, MPI_COMM_WORLD, status);
                MPI_Send(tasks, 2, MPI_INT, rank+1, task_tag, MPI_COMM_WORLD);
                System.out.printf("Comm %d sending energy %d to worker %d, goodbye!\n", rank, tasks[1], rank+1); 
            }
        }
        
        //code for worker
        else if (rank % 2 != 0) {
        	while (true){
        		//depending on worker location, request and receive from Comms
        		if (rank < size-2){
            		MPI_Send(buffer, 1, MPI_INT, rank+1, req_tag, MPI_COMM_WORLD);
            		MPI_Recv(tasks, 2, MPI_INT, rank+1, task_tag, MPI_COMM_WORLD, status);
        		}
        		else{
            		MPI_Send(buffer, 1, MPI_INT, rank-1, req_tag, MPI_COMM_WORLD);
            		MPI_Recv(tasks, 2, MPI_INT, rank-1, task_tag, MPI_COMM_WORLD, status);        			
        		}
        		//break if task is empty
        		//else do work
        		if (tasks[0] == -1){
        			break;
        		}
        		else{
        			Thread.sleep(rand.nextInt(2000));
        			tasks[1] = rank;
        			MPI_Send(tasks, 2, MPI_INT, size-1, task_tag, MPI_COMM_WORLD);
        		}
        	}
			MPI_Send(tasks, 2, MPI_INT, size-1, task_tag, MPI_COMM_WORLD);
			System.out.printf("Worker %d sending energy %d, goodbye!\n", rank, tasks[1]);        	
        }
        
    	//code for root(collector)
    	else{
    		int sum = 0;
        	while (sum != 30){
        		MPI_Recv(tasks, 2, MPI_INT, MPI_ANY_SOURCE, task_tag, MPI_COMM_WORLD, status);
        		if (tasks[0] != -1)
        			System.out.printf("Worker %d completed Task %d\n", status.MPI_SOURCE, tasks[0]);
        		else
        			sum += tasks[1];
        	}        		
    	}        
        
        MPI_Finalize();
    }
}
