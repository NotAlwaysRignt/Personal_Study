/**
public class TreeNode {
    int val = 0;
    TreeNode left = null;
    TreeNode right = null;

    public TreeNode(int val) {
        this.val = val;

    }

}
*/

//最简单的方法
import java.util.*;
public class Solution {
    boolean result = false;
    public boolean HasSubtree(TreeNode root1,TreeNode root2) {
        if(root2==null) return false;
        attachSameNodeWithB(root1,root2);
        return result;
    }
    
    private void attachSameNodeWithB(TreeNode root1,TreeNode root2){
        if(root1 == null) return;
        if(root1.val==root2.val){
            if(isSame(root1,root2))
                result=true;
        }
        attachSameNodeWithB(root1.left,root2);
        attachSameNodeWithB(root1.right,root2);            
    }
    
    private boolean isSame(TreeNode child,TreeNode root2){
        if(root2==null) return true;
        if(child==null){
            return false;
        }
        if(child.val!=root2.val){
            return false;
        }
        return isSame(child.left,root2.left)&&isSame(child.right,root2.right);
    }
}

//一开始想复杂了,用个辅助空间,其实就是把可能的节点添加到链表,稍后判断,但实际上当下就可以判断
/*
import java.util.*;
public class Solution {
    private List<TreeNode> list = new LinkedList<TreeNode>();
    
    public boolean HasSubtree(TreeNode root1,TreeNode root2) {
        if(root2==null) return false;
        attachSameNodeWithB(root1,root2);
        for(TreeNode child:list){
            if(isSame(child,root2)){
                return true;
            }
        }
        return false;
    }
    
    private void attachSameNodeWithB(TreeNode root1,TreeNode root2){
        if(root1 == null) return;
        if(root1.val==root2.val){
            list.add(root1);
        }
        attachSameNodeWithB(root1.left,root2);
        attachSameNodeWithB(root1.right,root2);            
    }
    
    private boolean isSame(TreeNode child,TreeNode root2){
        if(root2==null) return true;
        if(child==null){
            return false;
        }
        if(child.val!=root2.val){
            return false;
        }
        return isSame(child.left,root2.left)&&isSame(child.right,root2.right);
    }
}
*/
