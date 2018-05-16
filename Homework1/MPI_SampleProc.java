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
        
        if (rank == 0) {
        	for (int i = 1; i <= 30; i++) {
        		tasks[0] = i;
        		MPI_Recv(buffer, 1, MPI_INT, 2, req_tag, MPI_COMM_WORLD, status);
        		MPI_Send(tasks, 2, MPI_INT, 2, task_tag, MPI_COMM_WORLD);
        	}
        	MPI_Recv(buffer, 1, MPI_INT, 2, req_tag, MPI_COMM_WORLD, status);
        	MPI_Send(empty, 2, MPI_INT, 2, task_tag, MPI_COMM_WORLD);
        }
        
        else if (rank % 2 == 0 && rank != 0) {
            while (true) {
            	MPI_Send(buffer, 1, MPI_INT, rank-2, req_tag, MPI_COMM_WORLD);
            	MPI_Recv(tasks, 2, MPI_INT, rank-2, task_tag, MPI_COMM_WORLD, status);
            	if (buffer[0] == -1)
            		break;
            	else {
            		MPI_Recv(buffer, 1, MPI_INT, MPI_ANY_SOURCE, req_tag, MPI_COMM_WORLD, status);
            		MPI_Send(tasks, 2, MPI_INT, status.MPI_SOURCE, task_tag, MPI_COMM_WORLD);
            	}
            }
            MPI_Recv(buffer, 1, MPI_INT, MPI_ANY_SOURCE, req_tag, MPI_COMM_WORLD, status);
            MPI_Send(tasks, 2, MPI_INT, status.MPI_SOURCE, task_tag, MPI_COMM_WORLD);
        }
        
        else if (rank % 2 != 0 || rank == size-1) {
        	
        }
        
        MPI_Finalize();
    }
}
