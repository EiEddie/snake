Collision Detection: 碰撞检测
=============================

.. raw:: html

  <table class="fig"><tbody>
  <tr>
    <td class="d">0</td>
    <td></td>
    <td class="d"></td>
    <td class="r">3</td>
    <td class="l r"></td>
    <td class="l r"></td>
    <td class="l"></td>
  </tr>
  <tr>
    <td class="u d"></td>
    <td></td>
    <td class="u">2</td>
    <td></td><td></td><td></td>
    <td class="d">4</td>
  </tr>
  <tr>
    <td class="u"></td>
    <td class="r">1</td>
    <td class="l"></td>
    <td></td>
    <td class="r"><i>h</i></td>
    <td class="l">5</td>
    <td class="u"></td>
  </tr>
  </tbody></table>

图中蛇向左移动.

在所有与末节垂直的节中查找在蛇移动方向正前方的, 并计算末节与其的距离, 储存所有符合条件的距离的最小值.
特别地, 还应将垂直的节之前一节的末位置考虑在内, 如图中节(2)前一节, 即节(1)的末位置.
当蛇改变移动的方向或者末节被重新建立时, 应重新计算此距离.

其次, 当且仅当没有上述符合条件的节时, 应计算蛇头至前进方向的墙的距离.
当蛇改变移动方向时应重新计算此距离.

最终的距离即为按上述方法计算得出的值. 每当蛇前进一格时, 距离减1. 当距离减为0意味着发生碰撞.
