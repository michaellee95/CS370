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
        
        Random rand = new Random();

        int rank = MPI_Comm_rank(MPI_COMM_WORLD);
        int size = MPI_Comm_size(MPI_COMM_WORLD);
        
        int myValue[] = new int[size];
        int otherValue[] = new int[1];
        
        //fill array with random values
        //print number in array
        for (int i = 0; i < size; i++) {
        	myValue[i] = rand.nextInt(1000);
        	if (rank == 0)
        		System.out.print(myValue[i] + ", ");
        }
        
        MPI_Scatter(myValue, 1, MPI_INT, myValue, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        //iterate the even-odd compare-swap code
        for (int i = 0; i < size; i++) {
	        //check if even
	        if ((rank%2) == 0) {
	        	//check if right neighbor exist
	        	if (rank < size-1) {
	        		//send to right
	        		//receive from right
	        		MPI_Send(myValue, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
	        		MPI_Recv(otherValue, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, status);
	        		//check if received value is smaller
	        		if (otherValue[0] < myValue[0]) {
	        			myValue[0] = otherValue[0];
	        		}
	        	}
	        }
	        
	        //else if odd
	        else {
	        	//receive from left
	        	//send to left
	        	MPI_Recv(otherValue, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, status);
	        	MPI_Send(myValue, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
	        	//check if received value is smaller
	        	if (otherValue[0] > myValue[0]) {
	    			myValue[0] = otherValue[0];
	    		}
	        }
	        
	        //repeat except
	        //evens send left
	        //odds send right
	        //check if even
	        if ((rank%2) == 0) {
	        	if (rank > 0) {
	        		MPI_Send(myValue, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
	        		MPI_Recv(otherValue, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, status);
	        		if (otherValue[0] > myValue[0]) {
	        			myValue[0] = otherValue[0];
	        		}
	        	}
	        }
	        
	        else {
	        	if (rank < size-1) {
	            	MPI_Recv(otherValue, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, status);
	            	MPI_Send(myValue, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
	            	if (otherValue[0] < myValue[0]) {
	        			myValue[0] = otherValue[0];
	        		}        		
	        	}
	        }        
        }
      
        //gather values to root
        MPI_Gather(myValue, 1, MPI_INT, myValue, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank == 0) {
        	System.out.println();
            for (int i = 0; i < size; i++) {
            	System.out.print(myValue[i] + ", ");
            }        	
        }
        MPI_Finalize();
    }
}
